#include "../include/expertSystem.hpp"

static std::vector<int> facts(26, UNCERTAIN);
std::vector<int> memo;
static std::vector<Rule> rules;
static std::map<char, std::vector<int>> yesproducers, noproducers;
static std::vector<char> contras(26);

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
	if (facts[v - 'A'] == TRUE) { memo[v - 'A'] = TRUE; return true; }
	if (facts[v - 'A'] == FALSE) { memo[v - 'A'] = FALSE; return false; }
	if (memo[v - 'A'] == TRUE) return true;
	if (memo[v - 'A'] == FALSE) return false;
	bool yes = false, no = false;
	if (yesproducers.find(v) != yesproducers.end())
	{
		for (int idx : yesproducers[v])
		{
			Rule* r = &rules[idx];
			if (r->expr == nullptr) continue;
			if (evalExpr(r->expr)) { yes = true; break; }
		}
	}
	if (noproducers.find(v) != noproducers.end())
	{
		for (int idx : noproducers[v])
		{
			Rule* r = &rules[idx];
			if (r->expr == nullptr) continue;
			if (evalExpr(r->expr)) { no = true; break; }
		}
	}
	if (yes && no) contras[v - 'A'] = 1;
	if (yes) memo[v - 'A'] = TRUE;
	else if (no) memo[v - 'A'] = FALSE;
	return yes;
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

void printResult(void)
{
	for (int i = 0; i < 26; i++)
	{
		std::fill(memo.begin(), memo.end(), UNCERTAIN);
		char c = 'A' + static_cast<char>(i);
		solveChar(c);
		if (facts[i] == UNCERTAIN && memo[i] == TRUE) facts[c - 'A'] = memo[c - 'A'];
		if (memo[i] == TRUE) std::cout << c << " is true\t";
		else if (memo[i] == FALSE) std::cout << c << " is false\t";
		else std::cout << c << " is unknown\t";
		if (c == 'G' || c == 'N' || c == 'T' || c == 'Z') std::cout << std::endl;
	}
	for (int i = 0; i < 26; i++)
	{
		char c = 'A' + static_cast<char>(i);
		if (contras[i]) std::cout << RED << "[WARNING] Conditions for fact " << c << " is contradictory. State of fact set to true." << RESET << std::endl;
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
				facts[line[i] - 'A'] = TRUE;
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
			if (!isalpha(f[0]))
			{
				std::cout << RED << "Input not valid." << RESET << std::endl;
				continue;
			}
			char c = static_cast<char>(toupper(static_cast<unsigned char>(f[0])));
			std::cout << GREEN << "State of fact " << f << " has been changed." << RESET << std::endl;
			if (facts[c - 'A'] != TRUE) facts[c - 'A'] = TRUE;
			else facts[c - 'A'] = FALSE;
			printResult();
		}
		else break;
	}
	__Made in France__
}
