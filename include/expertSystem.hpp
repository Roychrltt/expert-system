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
	std::shared_ptr<Expr> left;
	std::shared_ptr<Expr> right;
	Expr (char o = 0, char v = 0, std::shared_ptr<Expr> l = nullptr, std::shared_ptr<Expr> r = nullptr): op(o), var(v), left(l), right(r) {}
};

struct Rule {
	std::shared_ptr<Expr> expr;
	char result;
	Rule (std::shared_ptr<Expr> e = nullptr, char r = 0): expr(e), result(r) {}
};

// ---------------------------- print --------------------------
void	orInConclusion(const std::string& line);
void	printUsage(void);
void	printHello(void);
void	printGoodbye(void);

// ---------------------------- utils --------------------------
std::string			trimLine(const std::string& line);
bool				checkLine(const std::string& line);
bool				isOp(char c);
int					precedence(char op);
std::vector<char>	tokenize(const std::string& s);
std::vector<char>	shuntingYard(const std::vector<char>& tokens);
std::shared_ptr<Expr>	buildAstFromRpn(const std::vector<char>& rpn);
