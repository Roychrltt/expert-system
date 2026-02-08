#include "../include/expertSystem.hpp"

std::string trimLine(const std::string& line)
{
	std::string ans = "";
	for (char c : line) if (c != ' ' && c != '\t') ans.push_back(c);
	return ans;
}

static bool beforeNeg(char c)
{
	return c == '(' || c == '+' || c == '|' || c == '^' || c == '>';
}
static bool allowed(char c)
{
	std::string s = "!|+<=>^()";
	if (s.find(c) == std::string::npos && !(c >= 'A' && c <= 'Z')) return false;
	return true;
}

bool checkLine(const std::string& line)
{
	if (line.empty()) return true;
	size_t n = line.size();
	if (line[0] == '<' || line[0] == '>' || line[0] == '|' || line[0] == '^' || line[0] == '+' || line[0] == ')') return false;
	if (line[0] == '=' || line[0] == '?')
	{
		for (size_t i = 1; i < n; i++)
			if (line[i] < 'A' || line[i] > 'Z') return false;
		return true;
	}
	int cnt = 0;
	int p = 0;
	for (size_t i = 0; i < n; i++)
	{
		char c = line[i];
		if (!allowed(c)) return false;
		if (i && c == '!' && !beforeNeg(line[i - 1])) return false;
		if (i && c >= 'A' && c <= 'Z' && !isOp(line[i - 1]) && line[i - 1] != '>' && line[i - 1] != '(') return false;
		if (i && (c == '+' || c == '^' || c == '|') && (line[i - 1] == '+' || line[i - 1] == '|' || line[i - 1] == '^' || line[i - 1] == '>')) return false;
		if (c == '<' && (i == n - 1 || line[i + 1] != '=')) return false;
		if (c == '=')
		{
			if (++cnt > 1) return false;
			if (i == n - 1 || (i && line[i + 1] != '>')) return false;
		}
		if (c == '(') p++;
		if (c == ')' && --p < 0) return false;
	}
	return p ? false : true;
}

bool isOp(char c)
{
	return c == '!' || c == '+' || c == '|' || c == '^';
}

int precedence(char op)
{
	if (op == '!') return 4;
	if (op == '+') return 3;
	if (op == '^') return 2;
	if (op == '|') return 1;
	return 0;
}

std::vector<char> tokenize(const std::string& s)
{
	std::vector<char> t;
	for (size_t i = 0; i < s.size(); i++)
	{
		char c = s[i];
		if ((c >= 'A' && c <= 'Z') || (isOp(c) || c == '(' || c == ')')) t.push_back(c);
	}
	return t;
}

std::vector<char> shuntingYard(const std::vector<char>& tokens)
{
	std::vector<char> out;
	std::vector<char> ops;
	for (char c : tokens)
	{
		if (c >= 'A' && c <= 'Z') out.push_back(c);
		else if (isOp(c))
		{
			if (c == '!')
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

std::shared_ptr<Expr> buildAstFromRpn(const std::vector<char>& rpn)
{
	std::vector<std::shared_ptr<Expr>> st;
	for (char c : rpn)
	{
		if (c >= 'A' && c <= 'Z')
			st.push_back(std::make_shared<Expr>(0, c, nullptr, nullptr));
		else if (isOp(c))
		{
			if (c == '!')
			{
				std::shared_ptr<Expr> a = nullptr;
				if (!st.empty())
				{
					a = st.back();
					st.pop_back();
				}
				st.push_back(std::make_shared<Expr>('!', 0, a, nullptr));
			}
			else
			{
				std::shared_ptr<Expr> a = nullptr, b = nullptr;
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
				st.push_back(std::make_shared<Expr>(c, 0, a, b));
			}
		}
	}
	if (st.empty()) return nullptr;
	return st.back();
}
