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

	constexpr static int dirsSize = 8;
	constexpr static int dirs[dirsSize] = { N, S, W, E, NW, NE, SW, SE };

	static const char* DirToString(int dir)
	{
		switch (dir)
		{
		case N: return "North";
		case S: return "South";
		case W: return "West";
		case E: return "East";
		case NW: return "Northwest";
		case NE: return "Northeast";
		case SW: return "Southwest";
		case SE: return "Southeast";
		default: return "";
		}		
	}
};

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Usage: ./this_program_name some_input_file.txt\n");
		// return -1;
		printf("Using default filename...\n");
	}

	std::fstream file(argc == 2 ? argv[1] : "small.txt");
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
		// if (idx > 0 && (idx - 1) / gridSideLength != row) printf("\n"); // separates every row
		printf("Current Character: %c\n", ch);
		printf("Current Index: %d\n", idx);
		(void)ch;

		auto FindValidDirections = [gridSideLength](int index)
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

				return allowedDirections;
			};

		// if I find the first character,
		// for all directions
		//   is neighbour valid?
		// 	   if so, search in that direction

		auto FindNeighbour = [gridSideLength](int index, int direction)
			{
				int neighbourIndex = index;

				if (direction & Direction::N)
					neighbourIndex -= gridSideLength;
				if (direction & Direction::S)
					neighbourIndex += gridSideLength;
				if (direction & Direction::W)
					neighbourIndex--;
				if (direction & Direction::E)
					neighbourIndex++;

				return neighbourIndex;
			};

		int validDirections = FindValidDirections(idx);
		printf("Valid directions: %x\n", validDirections);

		for (int i = 0; i < Direction::dirsSize; ++i)
		{
			int directionToCheck = Direction::dirs[i];

			// if `directionToCheck` has a '1' where there shouldn't be, this will return false
			bool isValid = (directionToCheck | validDirections) == validDirections;

			if (isValid)
			{
				int neighbourIndex = FindNeighbour(idx, directionToCheck);
				printf("%d: %s was valid, neighbour is %d\n", i, Direction::DirToString(directionToCheck), neighbourIndex);
			}
		}

		(void)SEQUENCE_LENGTH;
		(void)sequence;
		(void)total;
		(void)progress;
		printf("\n");
	}

	return 0;
}
