#include "../include/expertSystem.hpp"

void orInConclusion(const std::string& line)
{
	std::cerr << RED << "Parse error: OR exists in results." << RESET << std::endl;
	std::cerr << "Error line: " << line << std::endl;
	exit(1);
}

void contradiction(char c)
{
	std::cerr << "Knowledge for fact " << c << " is contradictory. Please verify your fact base." << std::endl;
	exit(1);
}

void printResult(void)
{
	for (int i = 0; i < 26; i++)
	{
		std::fill(memo.begin(), memo.end(), UNCERTAIN);
		char c = 'A' + (char)i;
		solveChar(c);
		if (memo[i] == TRUE) std::cout << c << " is true\t";
		else if (memo[i] == FALSE) std::cout << c << " is false\t";
		else std::cout << c << " is unknown\t";
		if (c == 'G' || c == 'N' || c == 'T' || c == 'Z') std::cout << std::endl;
	}
}

void printUsage(void)
{
	std::cerr << RED << "Error: wrong number of arguments." << RESET << std::endl;
	std::cerr << "Usage: ./expert_system <input_file>" << std::endl;
	exit(1);
}
