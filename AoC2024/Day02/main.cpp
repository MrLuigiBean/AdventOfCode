#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main_01()
{
	// Open file, exit if something goes wrong.
	std::ifstream file("input.txt", std::ios_base::in);
	if (!file)
		return -1;

	int counter = 0;

	// for every report in the file...
	while (true)
	{
		std::string report;

		// Read in the report + Exit file reading when EOF is reached.
		if (!std::getline(file, report))
			break;

		// std::cout << '|' << report << '|' << std::endl;

		std::vector<int> vec;

		// Store the report's numbers into a vector.
		for (std::stringstream sstream(report); sstream;)
		{
			int level;
			if (!(sstream >> level))
				break;
			vec.emplace_back(level);
		}

		// Determine initial direction for comparison later.
		const bool isAscending = vec[0] < vec[1];

		bool isSafe = true;
		for (int i = 0; i < vec.size() - 1; ++i)
		{
			// Ensure that the levels are either all increasing or all decreasing.
			if (vec[i] < vec[i + 1] != isAscending)
			{
				isSafe = false;
				break;
			}

			// Check that any two adjacent levels differ by at least one and at most three.
			int diff = vec[i] - vec[i + 1];
			if (diff > 3 || diff < -3 || diff == 0)
			{
				isSafe = false;
				break;
			}
		}

		if (isSafe)
			++counter;
	}

	printf("counter: %d\n", counter);

	file.close();

	return 0;
}

void PrintVec_02(const std::vector<int>& vec)
{
	printf("[");
	for (int i : vec)
		printf("%d ", i);
	printf("%c]\n", 0x08); // lmaooo backspace
}

bool IsRecordSafe_02(const std::vector<int>& vec, bool isOriginalLength = true)
{
	// Determine initial direction for comparison later.
	const bool isAscending = vec[0] < vec[1];

	bool isSafe = true;
	for (int i = 0; i < vec.size() - 1; ++i)
	{
		// Ensure that the levels are either all increasing or all decreasing.
		if (vec[i] < vec[i + 1] != isAscending)
			isSafe = false;

		// Check that any two adjacent levels differ by at least one and at most three.
		int diff = vec[i] - vec[i + 1];
		if (diff > 3 || diff < -3 || diff == 0)
			isSafe = false;
	}

	// If it wasn't immediately safe, give it a margin of error of 1 level.
	if (!isSafe && isOriginalLength)
	{
		for (int i = 0; i < vec.size(); ++i)
		{
			// try removing one by one I guess??
			std::vector<int> temp(vec);
			temp.erase(temp.begin() + i);

			isSafe = IsRecordSafe_02(temp, false);

			// If removing an element caused it to be safe, the original is safe!
			if (isSafe)
				break;
		}
	}

	return isSafe;
}

int main_02()
{
	// Open file, exit if something goes wrong.
	std::ifstream file("input.txt", std::ios_base::in);
	if (!file)
		return -1;

	int counter = 0;

	// for every report in the file...
	while (true)
	{
		std::string report;

		// Read in the report + Exit file reading when EOF is reached.
		if (!std::getline(file, report))
			break;

		std::vector<int> vec;

		// Store the report's numbers into a vector.
		for (std::stringstream sstream(report); sstream;)
		{
			int level;
			if (!(sstream >> level))
				break;
			vec.emplace_back(level);
		}

		if (IsRecordSafe_02(vec))
			++counter;
	}

	printf("counter: %d\n", counter);
	file.close();
	return 0;
}

int main()
{
	// main_01();
	main_02();
	return 0;
}
