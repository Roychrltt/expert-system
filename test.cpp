#include <bits/stdc++.h>

struct Expr
{
	char op;
	char var;
	Expr* left;
	Expr* right;
	Expr (char o = 0, char v = 0, Expr* l = nullptr, Expr* r = nullptr): op(o), var(v), left(l), right(r) {}
};

struct Rule {
	Expr* expr;
	char result;
	Rule (Expr* e = nullptr, char r = 0): expr(e), result(r) {}
};

static std::vector<int> initialFacts(26);
static std::map<char, int> stateMemo;
static std::vector<Rule> rules;
static std::map<char, std::vector<int>> producers;

static int precedence(char op)
{
	if (op == '!') return 4;
	if (op == '+') return 3;
	if (op == '^') return 2;
	if (op == '|') return 1;
	return 0;
}

static bool isOp(char c)
{
	return c=='!'||c=='+'||c=='|'||c=='^';
}

static std::vector<char> tokenize(const std::string& s)
{
	std::vector<char> t;
	for (size_t i = 0; i < s.size(); i++)
	{
		char c = s[i];
		if (c == ' ' || c == '\t' ) continue;
		if ((c >= 'A' && c <= 'Z') || (isOp(c) || c == '(' || c == ')')) t.push_back(c);
	}
	return t;
}

static Expr* buildAstFromRpn(const std::vector<char>& rpn) {
	std::vector<Expr*> st;
	for (char c : rpn)
	{
		if (c >= 'A' && c <= 'Z')
			st.push_back(new Expr(0, c, nullptr, nullptr));
		else if (isOp(c))
		{
			if (c == '!')
			{
				Expr* a = nullptr;
				if (!st.empty())
				{
					a = st.back();
					st.pop_back();
				}
				st.push_back(new Expr('!', 0, a, nullptr));
			}
			else
			{
				Expr* b = nullptr; Expr* a = nullptr;
				if (!st.empty())
				{
					b = st.back();
					st.pop_back();
				}
				if (!st.empty())
				{
					a = st.back();
					st.pop_back();
				}
				st.push_back(new Expr(c,0,a,b));
			}
		}
	}
	if (st.empty()) return nullptr;
	return st.back();
}

static std::vector<char> shuntingYard(const std::vector<char>& tokens) {
	std::vector<char> out;
	std::vector<char> ops;
	for (char c : tokens)
	{
		if (c >= 'A' && c <= 'Z')
			out.push_back(c);
		else if (isOp(c))
		{
			if (c=='!')
				ops.push_back(c);
			else
			{
				while (!ops.empty() && isOp(ops.back()) && (precedence(ops.back()) >= precedence(c)))
				{
					out.push_back(ops.back());
					ops.pop_back();
				}
				ops.push_back(c);
			}
		}
		else if (c == '(') ops.push_back(c);
		else if (c == ')')
		{
			while (!ops.empty() && ops.back() != '(')
			{
				out.push_back(ops.back());
				ops.pop_back();
			}
			if (!ops.empty() && ops.back() == '(') ops.pop_back();
		}
	}
	while (!ops.empty())
	{
		out.push_back(ops.back());
		ops.pop_back();
	}
	return out;
}

static bool evalExpr(Expr* e);

static bool solveChar(char v)
{
	if (initialFacts[v - 'A']) return true;

	int st = 0;
	if (stateMemo.count(v)) st = stateMemo[v];
	if (st == 2) return true;
	if (st == 1 || st == 3) return false;
	stateMemo[v] = 1;
	bool result = false;
	auto it = producers.find(v);
	if (it != producers.end())
	{
		for (int idx : it->second)
		{
			Rule* r = &rules[idx];
			if (r->expr == nullptr) continue;
			if (evalExpr(r->expr))
			{
				result = true;
				break;
			}
		}
	}
	stateMemo[v] = result ? 2 : 3;
	return result;
}

static bool evalExpr(Expr* e) {
	if (e == nullptr) return false;
	switch (e->op) {
		case 0:
			return solveChar(e->var);
			break;
		case '!':
			return !evalExpr(e->left);
			break;
		case '+':
			return evalExpr(e->left) && evalExpr(e->right);
			break;
		case '|':
			return evalExpr(e->left) || evalExpr(e->right);
			break;
		case '^':
			return evalExpr(e->left) ^ evalExpr(e->right);
			break;
		default:
			return false;
	}
}

static void addRuleFromLine(const std::string& line) {
	std::string s = line;
	size_t pos = s.find("<=>");
	bool equiv = false;
	if (pos != std::string::npos) equiv = true;
	else pos = s.find("=>");
	if (pos == std::string::npos) return;
	std::string left = s.substr(0,pos);
	std::string right = s.substr(pos + (equiv ? 3 : 2));
	auto parseSide = [&](const std::string& side)->Expr*
	{
		std::vector<char> tokens = tokenize(side);
		std::vector<char> rpn = shuntingYard(tokens);
		return buildAstFromRpn(rpn);
	};
	Expr* leftAst = parseSide(left);
	for (size_t i = 0; i < right.size(); ++i)
	{
		char c = right[i];
		if (c >= 'A' && c <= 'Z')
		{
			rules.emplace_back(leftAst, c);
			producers[c].push_back((int)rules.size() - 1);
		}
	}
	if (equiv)
	{
		Expr* rightAst = parseSide(right);
		for (size_t i = 0; i < left.size(); ++i)
		{
			char c = left[i];
			if (c >= 'A' && c <= 'Z')
			{
				rules.emplace_back(rightAst, c);
				producers[c].push_back((int)rules.size() - 1);
			}
		}
	}
}

int main(int argc, char** argv) {
	std::ifstream fin(argv[1]);
	std::string query;
	std::string line;
	while (std::getline(fin, line)) {
		size_t pos = line.find('#');
		std::string s = line.substr(0, pos);
		if (s.empty()) continue;
		if (s.find("=>") != std::string::npos || s.find("<=>") != std::string::npos)
		{
			addRuleFromLine(s);
			continue;
		}
		if (s[0] == '=') {
			for (size_t i=1;i<s.size();++i) {
				char c = s[i];
				if (c>='A' && c<='Z') initialFacts[c - 'A'] = true;
			}
			continue;
		}
		if (s[0] == '?') {
			query = line.substr(1);
			continue;
		}
	}
	for (char q : query) {
		stateMemo.clear();
		bool res = solveChar(q);
		if (res) std::cout << q << " is true\n";
		else std::cout << q << " is false\n";
	}
	return 0;
}
