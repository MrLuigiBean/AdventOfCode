#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define PRINT(x) std::cout << #x << ": " << (x) << "\n"

template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec)
{
	stream << '[';
	for (unsigned i = 0; i < vec.size(); ++i)
		stream << vec[i] << ((i < vec.size() - 1) ? "," : "");
	return stream << ']';
}

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

struct Update
{
	using Page = int;
	std::vector<Page> pages;
	std::vector<Rule> violatedRules;
	std::vector<Rule> followedRules;
	bool isCorrectlyOrdered = false;

	Update(const std::string& line)
	{
		std::stringstream sstream(line);
		Page pageNumber;
		char comma;
		do
		{
			sstream >> pageNumber;
			pages.emplace_back(pageNumber);
		} while (sstream >> comma);
	}

	struct Ranking
	{
		Page page = 0;
		int beforePoints = 0;
		int afterPoints = 0;
	};

	void FixIncorrectOrder();
};

inline std::ostream& operator<<(std::ostream& stream, const Update& update)
{
	return stream << update.pages;
}

inline std::ostream& operator<<(std::ostream& stream, const Update::Ranking& ranking)
{
	return stream << ranking.page << '(' << ranking.beforePoints << ',' << ranking.afterPoints << ')';
}

void Update::FixIncorrectOrder()
{
	std::vector<Rule> combinedRules;
	combinedRules.insert(combinedRules.end(), followedRules.begin(), followedRules.end());
	combinedRules.insert(combinedRules.end(), violatedRules.begin(), violatedRules.end());

	// ranking system??

	// based on the rules, rank each page for 
	// 'going in front' and 'going behind'

	// for each page, give them 'begin' points and 'end' points
	// use them to position the pages somehow??

	// data structure(?): page -> beforePts, afterPts

	std::vector<Ranking> rankings(pages.size());
	int index = 0;

	for (const Page page : pages)
	{
		Ranking& ranking = rankings[index];
		ranking.page = page;

		for (const Rule& rule : combinedRules)
			if (rule.before == page)
				++ranking.beforePoints;
			else if (rule.after == page)
				++ranking.afterPoints;

		++index;
	}

	std::sort(rankings.begin(), rankings.end(), [](const Ranking& a, const Ranking& b)
		{ return a.afterPoints < b.afterPoints; });

	for (unsigned i = 0; i < pages.size(); ++i)
		pages[i] = rankings[i].page;

	isCorrectlyOrdered = true;
}

bool ReadDataFromFile(const std::string& filename, std::vector<Rule>& rules, std::vector<Update>& updates)
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

void IdentifyIncorrectUpdates(std::vector<Update>& updates, const std::vector<Rule>& rules)
{
	for (Update& update : updates)
	{
		update.isCorrectlyOrdered = true;

		// create rules from update
		std::vector<Rule> generatedRules;
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

	std::vector<Rule> rules;
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

	std::vector<Rule> rules;
	std::vector<Update> updates;

	if (!ReadDataFromFile(filename, rules, updates))
		return -1;

	IdentifyIncorrectUpdates(updates, rules);

	std::erase_if(updates, [](const Update& update) { return update.isCorrectlyOrdered; });

	// fix 'em here
	for (Update& update : updates)
		update.FixIncorrectOrder();

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
