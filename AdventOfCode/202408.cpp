#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

#define PRINT(x) std::cout << #x << ": " << (x) << "\n"

/// @brief The grid of characters that store all location information from a file.
using Grid = std::vector<std::string>;

/// @brief Represents the x and y coordinate a location in the grid can have.
using Coord = std::pair<int, int>;

/// @brief The symbol used to represent empty locations.
constexpr char EMPTY_SYMBOL = '.';

/// @brief Prints a generic pair to a given stream.
/// @tparam T The type of the first element of the pair.
/// @tparam U The type of the second element of the pair.
/// @param stream The output stream to print to.
/// @param pair The pair to display using the given stream.
/// @return The modified stream with the pair's information printed.
template <typename T, typename U>
inline std::ostream& operator<<(std::ostream& stream, const std::pair<T, U>& pair)
{
	return stream << '(' << pair.first << ',' << pair.second << ')';
}

/// @brief Prints a std::vector to a given stream.
/// @tparam T The std::vector's value_type.
/// @param stream The output stream to print to.
/// @param vec The vector to display using the given stream.
/// @return The modified stream with the vector's information printed.
template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec)
{
	stream << '[';
	for (unsigned i = 0; i < vec.size(); ++i)
		stream << vec[i] << ((i < vec.size() - 1) ? "," : "");
	return stream << ']';
}

/// @brief Adds two coordinates together.
/// @param a The first coordinate.
/// @param b The second coordinate.
/// @return A new coordinate created from element-wise addition.
Coord operator+(const Coord& a, const Coord& b)
{
	return { a.first + b.first, a.second + b.second };
}

/// @brief Subtracts a coordinate from another.
/// @param a The first coordinate.
/// @param b The second coordinate.
/// @return A new coordinate created from subtracting the second from the first.
Coord operator-(const Coord& a, const Coord& b)
{
	return { a.first - b.first, a.second - b.second };
}

/// @brief Reads and stores information from the file into the given Grid.
/// @param filename The name of the file to obtain data from.
/// @param grid The grid that will be filled in with the information from the file.
/// @return true on success, false otherwise.
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

/// @brief Counts the number of antinodes in the grid.
/// @param grid The grid with information on the locations of antennas.
/// @return The number of antinodes within the grid's boundaries.
int CountBoundedAntinodes(const Grid& grid)
{
	std::map<char, std::vector<Coord>> frequencyLists;

	// store coordinates of all antennas
	for (unsigned row = 0; row < grid.size(); ++row)
		for (unsigned col = 0; col < grid[row].size(); ++col)
			if (grid[row][col] != EMPTY_SYMBOL)
				frequencyLists[grid[row][col]].emplace_back(row, col);

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

/// @brief Counts the number of antinodes from data in a given file.
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
