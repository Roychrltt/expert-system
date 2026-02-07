#include "../include/expertSystem.hpp"

void orInConclusion(const std::string& line)
{
	std::cerr << RED << "Parse error: OR exists in results." << RESET << std::endl;
	std::cerr << "Error line: " << line << std::endl;
	exit(1);
}

void printUsage(void)
{
	std::cerr << RED << "Error: wrong number of arguments." << RESET << std::endl;
	std::cerr << "Usage: ./expert_system <input_file>" << std::endl;
	exit(1);
}

void printHello(void)
{
	std::cout << CYAN << std::endl;
	std::cout <<
    " ███████╗██╗  ██╗██████╗ ███████╗██████╗ ████████╗\n"
    " ██╔════╝╚██╗██╔╝██╔══██╗██╔════╝██╔══██╗╚══██╔══╝\n"
    " █████╗   ╚███╔╝ ██████╔╝█████╗  ██████╔╝   ██║   \n"
    " ██╔══╝   ██╔██╗ ██╔═══╝ ██╔══╝  ██╔══██╗   ██║   \n"
    " ███████╗██╔╝ ██╗██║     ███████╗██║  ██║   ██║   \n"
    " ╚══════╝╚═╝  ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝   \n";
	std::cout << MAGENTA;
	std::cout <<
    " ███████╗██╗   ██╗███████╗████████╗███████╗███╗   ███╗\n"
    " ██╔════╝╚██╗ ██╔╝██╔════╝╚══██╔══╝██╔════╝████╗ ████║\n"
    " ███████╗ ╚████╔╝ ███████╗   ██║   █████╗  ██╔████╔██║\n"
    " ╚════██║  ╚██╔╝  ╚════██║   ██║   ██╔══╝  ██║╚██╔╝██║\n"
    " ███████║   ██║   ███████║   ██║   ███████╗██║ ╚═╝ ██║\n"
    " ╚══════╝   ╚═╝   ╚══════╝   ╚═╝   ╚══════╝╚═╝     ╚═╝\n";
	std::cout << RESET << std::endl;
	std::cout << "\t\t" << UNDER << "Made with ❤️" << RESET << std::endl;
	std::cout << std::endl;
}

void printGoodbye()
{
	std::cout << BOLD << GREEN << std::endl;
	std::cout << "\n========================================\n";
    std::cout << "  Thank you for using the Expert System  \n";
    std::cout << "        Have a wonderful day ✨          \n";
    std::cout << "========================================\n";
	std::cout << RESET << std::endl;
}
