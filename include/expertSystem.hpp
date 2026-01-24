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


// ---------------------------- print --------------------------
void	printUsage(void);
void	printGrid(const std::vector<int>& grid, size_t n);
void	printMoves(const std::vector<int>& originGrid, const std::string& move);
void	printSolution(unsigned long long int cnt, unsigned long long int mx, const std::string& move);

// ---------------------------- input --------------------------
void	readFile(const std::string &filename);
void	generateRandomPuzzle(int n, std::vector<int> &grid);

// ---------------------------- heuristics --------------------------
int		manhattan(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n);
int		linearConflict(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n);
int		hamming(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n, const std::vector<int>& goal);
int		chebyshev(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n);
int		euclidean(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n);

// ---------------------------- helper --------------------------
std::vector<int>	makeGoal(int n);
std::string			encode(const std::vector<int>& state);
int					parityPermutation(const std::vector<int>& grid);
bool				check(const std::vector<int>& grid, const std::vector<int>& goal, int n);
