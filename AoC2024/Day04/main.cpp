#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Direction
{
	const static int
		MT = 0b0000, // it sounds like 'empty' lol
		N = 0b0001,
		S = 0b0010,
		W = 0b0100,
		E = 0b1000,
		NW = N | W,
		NE = N | E,
		SW = S | W,
		SE = S | E;
};

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

	for (int idx = 0; idx < gridSize; idx++)
	{
		int row = idx / gridSideLength;
		int col = idx % gridSideLength;
		char ch = grid[row][col];
		if (idx > 0 && (idx - 1) / gridSideLength != row)
		{
			printf("\n");
		}
		// printf("Current Character: %c\n", ch);
		(void)ch;

		auto isNeighbourValid = [gridSideLength](int index)
			{
				int allowedDirections = Direction::MT;
				int gridSize = gridSideLength * gridSideLength;

				// is top ok?
				if (gridSideLength <= index && index <= gridSize - 1)
					allowedDirections |= Direction::N;

				// is bottom ok?
				if (0 <= index && index <= (gridSize - 1 - gridSideLength))
					allowedDirections |= Direction::S;

				// is left ok?
				if (index % gridSideLength != 0)
					allowedDirections |= Direction::W;

				// is right ok?
				if (index % gridSideLength != gridSideLength - 1)
					allowedDirections |= Direction::E;
				printf("%x", allowedDirections);
			};

		// if I find the first character,
		// for all directions
		//   is neighbour valid?
		// 	   if so, search in that direction

		isNeighbourValid(idx);
		(void)SEQUENCE_LENGTH;
		(void)sequence;
		(void)total;
		(void)progress;
	}

	return 0;
}
