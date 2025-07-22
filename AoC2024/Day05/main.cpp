#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define PRINT(x) std::cout << #x << ": " << (x) << "\n"

struct Rule
{
	int before, after;

	Rule(const std::string& line)
	{
		char pipe;
		std::stringstream(line) >> before >> pipe >> after;
	}

	Rule(const int before, const int after) : before{ before }, after{ after } {}

	bool operator==(const Rule& other) const
	{
		return before == other.before && after == other.after;
	}

	// sort based on first number, then second
	bool operator<(const Rule& other) const
	{
		// if (before < other.before)
		// 	return true;
		// else if (before == other.before)
		// 	return after < other.after;

		// ^ all this stuff is equal to the line below v

		return (before == other.before) ? (after < other.after) : (before < other.before);
	}
};

inline std::ostream& operator<<(std::ostream& stream, const Rule& rule)
{
	return stream << '(' << rule.before << '|' << rule.after << ')';
}

struct Rules : public std::vector<Rule>
{
	bool HasRule(Rule rule)
	{
		return std::find(begin(), end(), rule) != end();
	}
};

struct Update
{
	std::vector<int> pages;
	bool isCorrectlyOrdered = false;

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

	Rules rules;
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

	rules.shrink_to_fit();
	std::sort(rules.begin(), rules.end());

	updates.shrink_to_fit();

	for (Update& update : updates)
	{
		update.isCorrectlyOrdered = true;
		for (unsigned i = 0; i < update.pages.size(); ++i)
		{
			const int page = update.pages[i];
			(void)page;

			// if a page was already messed up, no need to continue checking other pages
			if (!update.isCorrectlyOrdered)
				break;

			bool isPageGood = true;

			;

			update.isCorrectlyOrdered &= isPageGood;
		}
	}

	std::erase_if(updates, [](const Update& update) { return !update.isCorrectlyOrdered; });

	int total = 0;
	for (const Update& update : updates)
	{
		total += update.pages[(update.pages.size() - 1) / 2];
	}

	PRINT(total);

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
