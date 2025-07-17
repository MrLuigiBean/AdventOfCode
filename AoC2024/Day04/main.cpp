#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Usage: ./this_program_name some_input_file.txt");
		return -1;
	}

	std::fstream file(argv[1]);
	if (!file)
	{
		printf("sorry %s isn't a file\n", argv[1]);
		return -1;
	}

	std::string line;
	std::getline(file, line);
	while (!isalpha(line.back()))
	{
		line.pop_back();
	}
	
	int gridSideLength = line.length();
	int gridSize = gridSideLength * gridSideLength;
	file.seekg(0);

	std::vector<std::string> grid(gridSideLength, std::string(gridSideLength, '\0'));
	{
		int i = 0;
		while (std::getline(file, line))
		{
			grid[i] = line;
			i++;
		}
	}

	const int SEQUENCE_LENGTH = 4;
	char sequence[] = "XMAS";
	int total = 0;
	int progress = 0;

	for (int i = 0; i < gridSize; i++)
	{
		int row = i / gridSideLength;
		int col = i % gridSideLength;
		char ch = grid[row][col];

		if (ch == sequence[progress])
		{
		}
	}

	return 0;
}
