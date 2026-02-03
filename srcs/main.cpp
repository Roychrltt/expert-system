#include "../include/expertSystem.hpp"

static std::vector<int> facts(26);
std::vector<int> memo;
static std::vector<Rule> rules;
static std::map<char, std::vector<int>> producers;

bool solveChar(char v);

bool evalExpr(std::shared_ptr<Expr> e)
{
	if (e == nullptr) return false;
	else if (!(e->op)) return solveChar(e->var);
	else if (e->op == '!') return !evalExpr(e->left);
	else if (e->op == '+') return evalExpr(e->left) && evalExpr(e->right);
	else if (e->op == '|') return evalExpr(e->left) || evalExpr(e->right);
	else if (e->op == '^') return evalExpr(e->left) ^ evalExpr(e->right);
	else return false;
}

bool solveChar(char v)
{
	if (facts[v - 'A'] == 1) { memo[v - 'A'] = TRUE; return true; }
	int st = 0;
	if (memo[v - 'A']) st = memo[v - 'A'];
	if (st == TRUE) return true;
	if (st == FALSE || st == UNCERTAIN) return false;
	memo[v - 'A'] = UNCERTAIN;
	bool result = false;
	if (producers.find(v) != producers.end())
	{
		for (int idx : producers[v])
		{
			Rule* r = &rules[idx];
			if (r->expr == nullptr) continue;
			if (evalExpr(r->expr)) { result = true; break; }
		}
	}
	memo[v - 'A'] = result ? TRUE : FALSE;
	return result;
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
	for (char c : res)
	{
		if (c >= 'A' && c <= 'Z')
		{
			rules.emplace_back(ast, c);
			producers[c].push_back((int)rules.size() - 1);
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
		std::cerr << "Error: input file open failure" << std::endl;
		exit(1);
	}

	memo.resize(26);
	std::string line;
	while (std::getline(file, line))
	{
		size_t pos = line.find('#');
		if (pos != std::string::npos) line = line.substr(0, pos);
		if (line.empty()) continue;

		if (line[0] == '=')
		{
			for (size_t i = 1; i < line.size(); i++)
				facts[line[i] - 'A'] = 1;
		}
		else if (line[0] == '?') continue;
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
	printResult();
	std::string q;
	while (1)
	{
		std::cout << YELLOW << "Would you like to change some facts to recheck the results? Y/N" << RESET << std::endl;
		if (!std::getline(std::cin, q)) break;
		if (!q.empty() && (q[0] == 'Y' || q[0] == 'y'))
		{
			std::cout << YELLOW << "Which fact would you like to change?" << RESET << std::endl;
			std::string f;
			if (!std::getline(std::cin, f)) break;
			std::cout << GREEN << "State of fact " << f << " has been changed." << RESET << std::endl;
		}
		else break;
	}
	__Made in France__
}
