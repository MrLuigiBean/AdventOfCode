#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#define PRINT(x) std::cout << #x << ": " << (x) << "\n"

using Grid = std::vector<std::string>;
using Coord = std::pair<int, int>;

constexpr char EMPTY_SYMBOL = '.';

template <typename T, typename U>
inline std::ostream& operator<<(std::ostream& stream, const std::pair<T, U>& pair)
{
	return stream << '(' << pair.first << ',' << pair.second << ')';
}

template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec)
{
	stream << '[';
	for (unsigned i = 0; i < vec.size(); ++i)
		stream << vec[i] << ((i < vec.size() - 1) ? "," : "");
	return stream << ']';
}

bool ReadDataFromFile(const std::string& filename, Grid& grid)
{
	std::fstream file(filename);
	if (!file)
	{
		printf("sorry %s isn't a file\n", filename.c_str());
		return false;
	}

	std::string line;
	std::getline(file, line);
	file.close();

	while (std::isspace(static_cast<unsigned char>(line.back())))
		line.pop_back();

	grid.clear();
	grid.reserve(line.size());

	file.open(filename);
	while (std::getline(file, line))
	{
		while (std::isspace(static_cast<unsigned char>(line.back())))
			line.pop_back();

		grid.emplace_back(line);
	}

	grid.shrink_to_fit();

	return true;
}

int CountBoundedAntinodes(const Grid& grid)
{
	std::map<char, std::vector<Coord>> frequencyLists;

	// store coordinates of all antennas
	for (unsigned i = 0; i < grid.size(); ++i)
		for (unsigned j = 0; j < grid[i].size(); ++j)
			if (grid[i][j] != EMPTY_SYMBOL)
				frequencyLists[grid[i][j]].emplace_back(i, j);

	for (const auto& frequencyList : frequencyLists)
	{
		printf("%c: ", frequencyList.first);
		std::cout << frequencyList.second << '\n';

		const std::vector<Coord>& coordinates = frequencyList.second;
		for (unsigned i = 0; i < coordinates.size() - 1; ++i)
		{
			for (unsigned j = i + 1; j < coordinates.size(); ++j)
			{
				auto first = coordinates[i];
				auto second = coordinates[j];
				PRINT(first);
				PRINT(second);
				printf("\n");

				//do computation :)
			}
		}

		printf("\n");
	}

	return grid.size();
}

int main(int argc, char* argv[])
{
	constexpr const char* defaultFilename = "small.txt";
	const char* filename = argc == 2 ? argv[1] : defaultFilename;

	if (argc != 2)
	{
		printf("Usage: ./this_program_name some_input_file.txt\n");
		// return -1;
		printf("Using default filename %s...\n", defaultFilename);
	}

	Grid grid;
	if (!ReadDataFromFile(filename, grid))
		return -1;

	int total = CountBoundedAntinodes(grid);
	PRINT(total);

	return 0;
}
