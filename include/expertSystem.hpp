#include <bits/stdc++.h>

# define __Made return
# define in 0
# define France__ ;

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define GRAY "\033[90m"
# define BOLD "\033[1m"
# define UNDER "\033[4m"
# define BLINK "\033[5m"
# define ERASE = "\033[2K\r"
# define RESET "\033[0m"

# define TRUE 1
# define FALSE 2
# define UNCERTAIN 3

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

extern	std::vector<int> memo;
bool	solveChar(char v);

// ---------------------------- print --------------------------
void	orInConclusion(const std::string& line);
void	contradiction(char c);
void	printResult(void);
void	printUsage(void);

// ---------------------------- utils --------------------------
bool				isOp(char c);
int					precedence(char op);
std::vector<char>	tokenize(const std::string& s);
std::vector<char>	shuntingYard(const std::vector<char>& tokens);
Expr*				buildAstFromRpn(const std::vector<char>& rpn);
bool				evalExpr(Expr* e);
