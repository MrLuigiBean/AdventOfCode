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
