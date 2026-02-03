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

void printUsage(void)
{
	std::cerr << RED << "Error: wrong number of arguments." << RESET << std::endl;
	std::cerr << "Usage: ./expert_system <input_file>" << std::endl;
	exit(1);
}
