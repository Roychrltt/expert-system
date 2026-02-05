#include "../include/expertSystem.hpp"

static std::string init;
static std::vector<int> facts(26, UNCERTAIN);
static std::vector<int> memo(26);
static std::vector<int> vis(26);
static std::vector<Rule> rules;
static std::map<char, std::vector<int>> yesproducers, noproducers;

int solveChar(char v);

int evalExpr(std::shared_ptr<Expr> e)
{
	if (e == nullptr) return FALSE;
	else if (!(e->op)) return solveChar(e->var);
	else
	{
		int l = evalExpr(e->left), r = evalExpr(e->right);
		if (e->op == '!')
		{
			if (l == TRUE) return FALSE;
			else if (l == FALSE) return TRUE;
			else return UNCERTAIN;
		}
		else if (e->op == '+')
		{
			if (l == TRUE && r == TRUE) return TRUE;
			else if (l == FALSE || r == FALSE) return FALSE;
			else return UNCERTAIN;
		}
		else if (e->op == '|')
		{
			if (l == TRUE || r == TRUE) return TRUE;
			else if (l == FALSE && r == FALSE) return FALSE;
			else return UNCERTAIN;
		}
		else if (e->op == '^')
		{
			if (l == TRUE && r == FALSE) return TRUE;
			else if (l == FALSE && r == TRUE) return TRUE;
			else if (l == TRUE && r == TRUE) return FALSE;
			else if (l == FALSE && r == FALSE) return FALSE;
			else return UNCERTAIN;
		}
		else return FALSE;
	}
}

int	solveChar(char v)
{
	int i = v - 'A';
	if (vis[i]) return memo[i];
	vis[i] = 1;
	if (facts[i] == TRUE) { memo[i] = TRUE; return TRUE; }
	if (facts[i] == FALSE) { memo[i] = FALSE; return FALSE; }
	if (memo[i] == TRUE) return TRUE;
	if (memo[i] == FALSE) return FALSE;
	bool yes = false, no = false;
	if (yesproducers.find(v) != yesproducers.end())
	{
		for (int idx : yesproducers[v])
		{
			Rule* r = &rules[idx];
			if (r->expr == nullptr) continue;
			if (evalExpr(r->expr) == TRUE) { yes = true; break; }
		}
	}
	if (noproducers.find(v) != noproducers.end())
	{
		for (int idx : noproducers[v])
		{
			Rule* r = &rules[idx];
			if (r->expr == nullptr) continue;
			if (evalExpr(r->expr) == TRUE) { no = true; break; }
		}
	}
	if (yes && !no) memo[i] = TRUE;
	else if (no && !yes) memo[i] = FALSE;
	return memo[i];
}

static void addRule(const std::string& con, const std::string& res)
{
	auto parseSide = [&](const std::string& s) -> std::shared_ptr<Expr>
	{
		std::vector<char> tokens = tokenize(s);
		std::vector<char> rpn = shuntingYard(tokens);
		return buildAstFromRpn(rpn);
	};

	std::shared_ptr<Expr> ast = parseSide(con);
	for (size_t i = 0; i < res.size(); i++)
	{
		char c = res[i];
		if (c >= 'A' && c <= 'Z' && i && res[i - 1] == '!')
		{
			rules.emplace_back(ast, c);
			noproducers[c].push_back((int)rules.size() - 1);
		}
		else if (c >= 'A' && c <= 'Z')
		{
			rules.emplace_back(ast, c);
			yesproducers[c].push_back((int)rules.size() - 1);
		}
	}
}

int main(int ac, char** av)
{
	if (ac != 2) printUsage();

	std::string input = av[1];
	std::ifstream file(input);
	if (!file.is_open())
	{
		std::cerr << RED << "Error: input file open failure" << RESET << std::endl;
		return 1;
	}

	std::string line;
	std::string tocheck;
	while (std::getline(file, line))
	{
		line = trimLine(line);
		size_t pos = line.find('#');
		if (pos != std::string::npos) line = line.substr(0, pos);
		if (line.empty()) continue;

		if (line[0] == '=')
		{
			init = line.substr(1);
			for (size_t i = 1; i < line.size(); i++)
				facts[line[i] - 'A'] = TRUE;
		}
		else if (line[0] == '?')
		{
			tocheck = line.substr(1);
			continue;
		}
		else
		{
			pos = line.find("=>");
			std::string con = line.substr(0, pos - 1);
			std::string res = line.substr(pos + 2);

			if (res.find('|') != std::string::npos || res.find('(') != std::string::npos)
				orInConclusion(line);
			addRule(con, res);

			if (line[pos - 1] == '<')
			{
				if (con.find('|') != std::string::npos || con.find('(') != std::string::npos)
					orInConclusion(line);
				addRule(res, con);
			}
		}
	}
	std::sort(tocheck.begin(), tocheck.end());
	std::string res = "?";
	for (char c : tocheck)
	{
		std::fill(memo.begin(), memo.end(), UNCERTAIN);
		std::fill(vis.begin(), vis.end(), 0);
		solveChar(c);
		int i = c - 'A';
		if (facts[i] == UNCERTAIN && memo[i] != UNCERTAIN) facts[i] = memo[i];
		if (memo[i] == TRUE) res += c;
	}
	std::cout << res << std::endl;
	__Made in France__
}
