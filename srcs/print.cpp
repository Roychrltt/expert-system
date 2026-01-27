#include "../include/expertSystem.hpp"

void	printUsage(void)
{
	std::cout << "Usage: ./n_puzzle [options]\n\n";
	std::cout << "Options:\n";
	std::cout << "  --filename <file>        Load puzzle from input file\n";
	std::cout << "  --random <n>             Generate a random solvable puzzle of size n x n\n";
	std::cout << "  --heuristic <type>       Choose heuristic function (default: manhattan)\n";
	std::cout << "                           Options: manhattan, linear_conflict, hamming\n";
	std::cout << "  --solver <type>          Choose search algorithm (default: a_star)\n";
	std::cout << "                           Options: a_star, greedy, uniform_cost\n";
	std::cout << "  --verbose                Print each step of the solution\n";
	std::cout << "  --benchmark              Solve puzzle with every heuristic function and a* variant\n";
	std::cout << "                           to have a comprehensive understanding of each method\n";
	std::cout << "  --help, -h               Show this help message and exit\n";
	std::cout << "\nExamples:\n";
	std::cout << " ./n-puzzle --filename puzzle.txt --heuristic linear_conflict --solver greedy\n";
	std::cout << " ./n-puzzle --random 3 --heuristic manhattan --solver uniform_cost\n";
}

void printGrid(const std::vector<int>& grid, size_t n)
{
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			std::cout << grid[i * n + j] << " ";
		}
		std::cout << std::endl;
	}
}

void	printMoves(const std::vector<int>& originGrid, const std::string& move)
{
	std::vector<int> grid = originGrid;
	size_t n = static_cast<size_t>(std::sqrt(grid.size()));
	printGrid(grid, n);
	size_t idx = 0;
	while (idx < grid.size() && grid[idx] != 0)
		idx++;
	for (auto& c : move)
	{
		if (c == 'U')
		{
			std::cout << "\n\t\u2191\n" << std::endl;
			std::swap(grid[idx], grid[idx - n]);
			idx -= n;
		}
		else if (c == 'D')
		{
			std::cout << "\n\t\u2193\n" << std::endl;
			std::swap(grid[idx], grid[idx + n]);
			idx += n;
		}
		else if (c == 'L')
		{
			std::cout << "\n\t\u2190\n" << std::endl;
			std::swap(grid[idx], grid[idx - 1]);
			idx--;
		}
		else if (c == 'R')
		{
			std::cout << "\n\t\u2192\n" << std::endl;
			std::swap(grid[idx], grid[idx + 1]);
			idx++;
		}
		printGrid(grid, n);
	}
}

void	printSolution(unsigned long long int cnt, unsigned long long int mx, const std::string& move)
{
	std::cout << MAGENTA << BOLD << "Time complexity: " << UNDER << cnt << RESET << MAGENTA << " states selected in total" << std::endl;
	std::cout << MAGENTA << BOLD << "Size complexity: " << RESET << MAGENTA << "at most " << BOLD << UNDER << mx << RESET << MAGENTA << " states ever represented in memory at the same time" << std::endl;
	std::cout << MAGENTA << BOLD << "Number of moves to achieve the final state: " << UNDER << move.size() << RESET << std::endl;
	std::cout << MAGENTA << BOLD << "Solution: " << RESET << MAGENTA << move << RESET << std::endl;
}
