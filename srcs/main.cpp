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

void printResult(const std::vector<int>& state, const std::string& query)
{
	for (char c : query)
	{
		if (state[c - 'A'] == 1) std::cout << c << " is true" << std::endl;
		else if (state[c - 'A'] == -1) std::cout << c << " is false" << std::endl;
		else std::cout << c << " is unknown" << std::endl;
	}
}

int main(int ac, char** av)
{
	if (ac != 2)
	{
		std::cerr << RED << "Error: wrong number of arguments." << RESET << std::endl;
		std::cerr << "Usage: ./expert_system <input_file>" << std::endl;
		return 0;
	}
	std::string input = av[1];

	std::ifstream file(input);
	if (!file.is_open())
	{
		std::cerr << "Error: input file open failure" << std::endl;
		exit(1);
	}

	std::string truth, query;
	std::vector<std::pair<std::string, std::string>> rules;
	std::string line;
	while (std::getline(file, line))
	{
		size_t pos = line.find('#');
		if (pos != std::string::npos) line = line.substr(0, pos);
		if (line.empty()) continue;

		if (line[0] == '=') truth = line.substr(1);
		else if (line[0] == '?') query = line.substr(1);
		else
		{
			size_t p = line.find('=');
			std::string con = line.substr(0, p - 1);
			std::string res = line.substr(p + 2);

			if (res.find('|') != std::string::npos || res.find('(') != std::string::npos)
				orInConclusion(line);
			rules.emplace_back(con, res);

			if (line[p - 1] == '<')
			{
				if (con.find('|') != std::string::npos || con.find('(') != std::string::npos)
					orInConclusion(line);
				rules.emplace_back(res, con);
			}
		}
	}
	std::vector<int> state(26);
	for (char c : truth) state[c - 'A'] = 1;
	auto evaluate = [&](this auto&& evaluate, const std::string& rule) -> bool
	{

	};
	bool flag = false;
	while (flag)
	{
		flag = false;
		for (const auto& [con, res])
		{
			if (evalueste(con))
			{
				std::string word;
				std::stringstream ss(res);
				while (ss >> word)
				{
					if (word.length() == 1 && word[0] >= 'A' && word[0] <= 'Z')
					{
						char c = word[0] - 'A';
						if (state[c] == 0)
						{
							flag = true;
							state[c] = 1;
						}
						else if (state[c] == -1) contradiction(c);
					}
					else if (word[0] == '!' && word[1] >= 'A' && word[1] <= 'Z')
					{
						char c = word[1] - 'A';
						if (state[c] == 0)
						{
							flag = true;
							state[c] = -1;
						}
						else if (state[c] == 1) contradiction(c);
					}
				}
			}
		}
		if (!flag) break;
	}
	printResult(state, query);
	__Made in France__
}
