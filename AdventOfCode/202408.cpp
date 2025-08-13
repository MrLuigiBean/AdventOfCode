#include <iostream>
#include <fstream>
#include <map>
#include <set>
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

Coord operator+(const Coord& a, const Coord& b)
{
	return { a.first + b.first, a.second + b.second };
}

Coord operator-(const Coord& a, const Coord& b)
{
	return { a.first - b.first, a.second - b.second };
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

	auto IsInsideBounds = [&grid](const Coord& coord)
		{
			int size = grid.size();
			return 0 <= coord.first && coord.first <= size - 1 &&
				0 <= coord.second && coord.second <= size - 1;
		};

	// disallow duplicate antinodes at the same coordinates
	std::set<Coord> total;

	for (const auto& frequencyList : frequencyLists)
	{
		const std::vector<Coord>& coordinates = frequencyList.second;
		for (unsigned i = 0; i < coordinates.size() - 1; ++i)
		{
			for (unsigned j = i + 1; j < coordinates.size(); ++j)
			{
				const Coord& first = coordinates[i];
				const Coord& second = coordinates[j];
				const Coord& translation = second - first; // final - initial

				Coord antinode1 = second + translation;
				if (IsInsideBounds(antinode1))
					total.emplace(antinode1);

				Coord antinode2 = first - translation;
				if (IsInsideBounds(antinode2))
					total.emplace(antinode2);
			}
		}
	}

	return total.size();
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
