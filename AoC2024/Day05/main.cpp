#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Rule
{
	int before, after;

	Rule(const std::string& line)
	{
		char pipe;
		std::stringstream(line) >> before >> pipe >> after;
	}
};

inline std::ostream& operator<<(std::ostream& stream, const Rule& rule)
{
	return stream << '(' << rule.before << '|' << rule.after << ')';
}

struct Update
{
	std::vector<int> pages;

	Update(const std::string& line)
	{
		std::stringstream sstream(line);
		int pageNumber;
		char comma;
		do
		{
			sstream >> pageNumber;
			pages.emplace_back(pageNumber);
		} while (sstream >> comma);
	}
};

inline std::ostream& operator<<(std::ostream& stream, const Update& update)
{
	stream << '[';
	for (unsigned i = 0; i < update.pages.size(); ++i)
		stream << update.pages[i] << ((i < update.pages.size() - 1) ? ',' : '\0');
	return stream << ']';
}

int main_01(int argc, char* argv[])
{
	const char* defaultFilename = "small.txt";
	if (argc != 2)
	{
		printf("Usage: ./this_program_name some_input_file.txt\n");
		// return -1;
		printf("Using default filename %s...\n", defaultFilename);
	}

	std::fstream file(argc == 2 ? argv[1] : defaultFilename);
	if (!file)
	{
		printf("sorry %s isn't a file\n", argv[1]);
		return -1;
	}

	std::vector<Rule> rules;
	bool isReadingRules = true;

	std::vector<Update> updates;
	bool isReadingUpdates = false;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.length() == 0)
		{
			isReadingRules = false;
			isReadingUpdates = true;
			continue;
		}

		if (isReadingRules)
			rules.emplace_back(line);

		if (isReadingUpdates)
			updates.emplace_back(line);
	}

	printf("All rules:\n");
	for (const Rule& rule : rules) std::cout << rule << '\n';

	printf("All updates:\n");
	for (const Update& update : updates) std::cout << update << '\n';

	return 0;
}

int main_02(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	printf("Hello 2!\n");
	return 0;
}

int main(int argc, char* argv[])
{
	main_01(argc, argv);
	// main_02(argc, argv);
	return 0;
}
