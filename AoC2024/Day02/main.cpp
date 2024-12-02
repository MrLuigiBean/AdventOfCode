#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main()
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
