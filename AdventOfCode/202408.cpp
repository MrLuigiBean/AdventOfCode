#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#define PRINT(x) std::cout << #x << ": " << (x) << "\n"

using Grid = std::vector<std::string>;

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

	printf("hello world!\n");
	return 0;
}
