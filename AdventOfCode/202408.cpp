#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

#define PRINT(x) std::cout << #x << ": " << (x) << "\n"

/// @brief The grid of characters that store all location information from a file.
using Grid = std::vector<std::string>;

/// @brief The symbol used to represent empty locations.
constexpr char EMPTY_SYMBOL = '.';

/// @brief Whether to account for resonant harmonics. Used to separate Part 1 from Part 2.
constexpr bool accountForHarmonics = false;

/// @brief Represents the x and y coordinate a location in the grid can have.
struct Coord
{
	/// @brief The row of this coordinate.
	int row = 0;

	/// @brief The column of this coordinate.
	int col = 0;

	/// @brief Assigns 0 to row and col.
	Coord() = default;

	/// @brief Assignes the row and col of this object.
	/// @param row_ The value of row.
	/// @param col_ The value of col.
	Coord(int row_, int col_) : row{ row_ }, col{ col_ } {}

	/// @brief Adds this and another coordinate together.
	/// @param other The other coordinate.
	/// @return A new coordinate created from element-wise addition.
	Coord operator+(const Coord& other) const
	{
		return { row + other.row, col + other.col };
	}

	/// @brief Subtracts another coordinate from this.
	/// @param other The other coordinate to subtract.
	/// @return A new coordinate created from subtracting the other from this object.
	Coord operator-(const Coord& other) const
	{
		return { row - other.row, col - other.col };
	}

	/// @brief Compares two objects using their row and col.
	/// @param other The other object to compare to.
	/// @return true Compares row from both objects. In the event of a tie, col is compared.
	bool operator<(const Coord& other) const
	{
		// if (row == other.row)
		// 	return col < other.col;
		// else
		// 	return row < other.row;

		// ^ same as this v
		return (row == other.row) ? (col < other.col) : (row < other.row);
	}
};

/// @brief Prints a coordinate to a given stream.
/// @param stream The output stream to print to.
/// @param coord The coordinates to display using the given stream.
/// @return The modified stream with the coordinate's information printed.
inline std::ostream& operator<<(std::ostream& stream, const Coord& coord)
{
	return stream << '(' << coord.row << ',' << coord.col << ')';
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
			return 0 <= coord.row && coord.row <= size - 1 &&
				0 <= coord.col && coord.col <= size - 1;
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

				if (!accountForHarmonics) // Part 1
				{
					Coord antinode1 = second + translation;
					if (IsInsideBounds(antinode1))
						total.emplace(antinode1);

					Coord antinode2 = first - translation;
					if (IsInsideBounds(antinode2))
						total.emplace(antinode2);
				}
				else // Part 2
				{
					;
				}
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
