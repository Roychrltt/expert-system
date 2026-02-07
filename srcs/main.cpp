#include "../include/expertSystem.hpp"

static std::string init;
static std::vector<int> facts(26, UNCERTAIN);
static std::vector<int> memo(26);
static std::vector<int> vis(26);
static std::vector<Rule> rules;
static std::map<char, std::vector<int>> yesproducers, noproducers;
static std::vector<char> contras(26);

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
		for (intid x : yesproducers[v])
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
	if (yes && no)
	{
		contras[i] = 1;
		memo[i] = UNCERTAIN;
	}
	else if (yes && !no) memo[i] = TRUE;
	else if (!yes && no) memo[i] = FALSE;
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

void printResult(void)
{
	std::cout << BLUE << BOLD << "\nState of facts after reasoning:" << RESET << std::endl;
	std::fill(contras.begin(), contras.end(), 0);
	for (int i = 0; i < 26; i++)
	{
		std::fill(memo.begin(), memo.end(), UNCERTAIN);
		std::fill(vis.begin(), vis.end(), 0);
		char c = 'A' + static_cast<char>(i);
		solveChar(c);
		if (facts[i] == UNCERTAIN && memo[i] != UNCERTAIN) facts[c - 'A'] = memo[c - 'A'];
		if (memo[i] == TRUE) std::cout << c << " is true\t";
		else if (memo[i] == FALSE) std::cout << c << " is false\t";
		else std::cout << c << " is unknown\t";
		if (c == 'G' || c == 'N' || c == 'T' || c == 'Z') std::cout << std::endl;
	}
	for (int i = 0; i < 26; i++)
	{
		char c = 'A' + static_cast<char>(i);
		if (contras[i]) std::cout << RED << "[WARNING] Conditions for fact " << c << " is contradictory. State of fact set to uncertain." << RESET << std::endl;
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

	printHello();
	std::cout << YELLOW << BOLD << "Below is the input file:" << RESET << std::endl;
	std::cout << std::endl;
	std::string line;
	while (std::getline(file, line))
	{
		std::cout << line << std::endl;
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
		std::cout << BOLD << GREEN << "Current initial facts: " << init << RESET << std::endl;
		std::cout << BOLD << YELLOW << "Would you like to change some initial facts to recheck the results? Y/N" << std::endl;
		std::cout << "(True facts will be set to uncertain and others will be set to true)" << RESET << std::endl;
		if (!std::getline(std::cin, q)) break;
		if (!q.empty() && (q[0] == 'Y' || q[0] == 'y'))
		{
			std::cout << BOLD << YELLOW << "Which facts would you like to change?" << RESET << std::endl;
			std::string f;
			if (!std::getline(std::cin, f) || f.empty()) break;
			for (char &c : f)
			{
				if (!isalpha(c))
				{
					std::cout << RED << "Input not valid." << RESET << std::endl;
					continue;
				}
				c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
			}
			std::vector<int> v(26);
			for (char c : init) v[c - 'A'] = 1;
			for (char c : f) v[c - 'A'] ^= 1;
			std::string tmp = "";
			for (int i = 0; i < 26; i++)
				if (v[i]) tmp += static_cast<char>('A' + i);
			init = tmp;
			std::fill(facts.begin(), facts.end(), UNCERTAIN);
			for (int i = 0; i < 26; i++)
				if (v[i]) facts[i] = TRUE;
			printResult();
		}
		else break;
	}
	printGoodbye();
	__Made in France__
}
