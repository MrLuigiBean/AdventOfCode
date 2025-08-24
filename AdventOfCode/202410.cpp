#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define PRINT(x) std::cout << #x << ": " << (x) << "\n"

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

/// @brief Represents the heights of all steps in a given map.
using StepHeights = std::vector<std::vector<int>>;

/// @brief Indicies (coordinates) for elements in 2D arrays.
struct Idx2D
{
	/// @brief The row of this coordinate.
	int row = 0;

	/// @brief The column of this coordinate.
	int col = 0;

	/// @brief Assigns 0 to row and col.
	Idx2D() = default;

	/// @brief Assignes the row and col of this object.
	/// @param row_ The value of row.
	/// @param col_ The value of col.
	Idx2D(int row_, int col_) : row{ row_ }, col{ col_ } {}

	/// @brief Represents the modifications that can be made to an instance of Idx2D.
	struct IfArgs
	{
		/// @brief The desired modification to the instance's row.
		int modRow = 0;

		/// @brief The desired modification to the instance's column.
		int modCol = 0;
	};

	/// @brief Creates a new Idx2D, which is a copy of this instance with modifications.
	/// @param args The modifications to apply.
	/// @return A copy of the instance with the modifications applied.
	inline Idx2D If(const IfArgs& args) const
	{
		return Idx2D(row + args.modRow, col + args.modCol);
	}
};

/// @brief Prints a coordinate to a given stream.
/// @param stream The output stream to print to.
/// @param coord The coordinates to display using the given stream.
/// @return The modified stream with the coordinate's information printed.
inline std::ostream& operator<<(std::ostream& stream, const Idx2D& coord)
{
	return stream << '(' << coord.row << ',' << coord.col << ')';
}

/// @brief Reads and stores information from the file into the given parameter.
/// @param filename The name of the file to obtain data from.
/// @param stepHeights The grid of step heights populated from the file.
/// @return true on success, false otherwise.
bool ReadDataFromFile(const std::string& filename, StepHeights& stepHeights)
{
	std::fstream file(filename);
	if (!file)
	{
		printf("sorry %s isn't a file\n", filename.c_str());
		return false;
	}

	std::string line;
	while (std::getline(file, line))
	{
		while (std::isspace(line.back()))
			line.pop_back();

		std::vector<int> temp;
		for (const char ch : line)
			temp.emplace_back(ch - '0');
		temp.shrink_to_fit();
		stepHeights.emplace_back(temp);
	}

	stepHeights.shrink_to_fit();

	return true;
}

/// @brief
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

	StepHeights stepHeights;
	if (!ReadDataFromFile(filename, stepHeights))
		return -1;

	PRINT(stepHeights);

	printf("Hello world!\n");

	return 0;
}
