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

	void Flip() { std::swap(before, after); }
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
	std::vector<Rule> violatedRules;
	std::vector<Rule> followedRules;
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

template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec)
{
	stream << '[';
	for (unsigned i = 0; i < vec.size(); ++i)
		stream << vec[i] << ((i < vec.size() - 1) ? "," : "");
	return stream << ']';
}

inline std::ostream& operator<<(std::ostream& stream, const Update& update)
{
	return stream << update.pages;
}

bool ReadDataFromFile(const std::string& filename, Rules& rules, std::vector<Update>& updates)
{
	std::fstream file(filename);
	if (!file)
	{
		printf("sorry %s isn't a file\n", filename.c_str());
		return false;
	}

	bool isReadingRules = true;
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

	return true;
}

void IdentifyIncorrectUpdates(std::vector<Update>& updates, const Rules& rules)
{
	for (Update& update : updates)
	{
		update.isCorrectlyOrdered = true;

		// create rules from update
		Rules generatedRules;
		for (unsigned i = 0; i < update.pages.size(); ++i)
		{
			for (unsigned j = i + 1; j < update.pages.size(); ++j)
			{
				Rule generatedRule(update.pages[i], update.pages[j]);
				generatedRules.emplace_back(generatedRule);
				if (std::find(rules.begin(), rules.end(), generatedRule) != rules.end())
					update.followedRules.emplace_back(generatedRule);
			}
		}

		// see if there are contradicting rules from before
		std::for_each(generatedRules.begin(), generatedRules.end(), [](Rule& rule) { rule.Flip(); });

		for (const Rule& flippedRule : generatedRules)
		{
			if (std::find(rules.begin(), rules.end(), flippedRule) != rules.end())
			{
				update.isCorrectlyOrdered = false;
				update.violatedRules.emplace_back(flippedRule);
			}
		}
	}
}

int main_01(int argc, char* argv[])
{
	constexpr const char* defaultFilename = "small.txt";
	const char* filename = argc == 2 ? argv[1] : defaultFilename;

	if (argc != 2)
	{
		printf("Usage: ./this_program_name some_input_file.txt\n");
		// return -1;
		printf("Using default filename %s...\n", defaultFilename);
	}

	Rules rules;
	std::vector<Update> updates;

	if (!ReadDataFromFile(filename, rules, updates))
		return -1;

	IdentifyIncorrectUpdates(updates, rules);

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
	constexpr const char* defaultFilename = "small.txt";
	const char* filename = argc == 2 ? argv[1] : defaultFilename;

	if (argc != 2)
	{
		printf("Usage: ./this_program_name some_input_file.txt\n");
		// return -1;
		printf("Using default filename %s...\n", defaultFilename);
	}

	Rules rules;
	std::vector<Update> updates;

	if (!ReadDataFromFile(filename, rules, updates))
		return -1;

	IdentifyIncorrectUpdates(updates, rules);

	std::erase_if(updates, [](const Update& update) { return update.isCorrectlyOrdered; });

	// fix 'em here
	for (Update& update : updates)
	{
		std::cout << "Current update: " << update << '\n';
		std::cout << "Followed rules: " << update.followedRules << '\n';
		std::cout << "Violated rules: " << update.violatedRules << '\n';

		// i'll be shocked if this works
		// update: it didn't, 6633 was too high
#if 0
		for (const Rule& rule : update.violatedRules)
		{
			std::cout << "Current rule: " << rule << '\n';
			std::swap(
				*(std::find(update.pages.begin(), update.pages.end(), rule.before)),
				*(std::find(update.pages.begin(), update.pages.end(), rule.after)));
			std::cout << update << '\n';
		}
#endif

		printf("\n");
	}

	int total = 0;
	for (const Update& update : updates)
	{
		total += update.pages[(update.pages.size() - 1) / 2];
	}

	PRINT(total);

	return 0;
}

int main(int argc, char* argv[])
{
	// return main_01(argc, argv);
	return main_02(argc, argv);
}
