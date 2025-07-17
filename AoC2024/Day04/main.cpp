#include <iostream>
#include <fstream>
#include <functional>
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

	static int gridSideLength;

	static int FindValidDirections(int index)
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

	// if `directionToCheck` has a '1' where `validDirections` doesn't, this will return false
	inline static bool IsValidDirection(int validDirections, int directionToCheck)
	{
		return (directionToCheck | validDirections) == validDirections;
	};

	static int GetNeighbour(int index, int direction)
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
};

int Direction::gridSideLength = 0;

int main_01(int argc, char* argv[])
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
	Direction::gridSideLength = gridSideLength;
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
	char sequence[] = { 'X','M','A','S' };
	int total = 0;

	for (int idx = 0; idx < gridSize; idx++)
	{
		int row = idx / gridSideLength;
		int col = idx % gridSideLength;
		char ch = grid[row][col];
		printf("Current Character: %c\n", ch);
		printf("Current Index: %d\n", idx);

		if (ch != sequence[0])
			continue;

		int validDirections = Direction::FindValidDirections(idx);
		printf("Valid directions: %x\n", validDirections);

		for (int i = 0; i < Direction::dirsSize; ++i)
		{
			int directionToCheck = Direction::dirs[i];

			if (Direction::IsValidDirection(validDirections, directionToCheck))
			{
				int neighbourIndex = Direction::GetNeighbour(idx, directionToCheck);
				printf("%d: %s was valid, neighbour is %d\n", i, Direction::DirToString(directionToCheck), neighbourIndex);

				// using `std::function` because it allows for recursive calls, unlike `auto`
				// thanks to https://stackoverflow.com/q/7861506/30412497 !
				std::function<bool(int, int, int)> FindRestOfSequence = [&](int neighbour, int direction, int progress) -> bool
					{
						int row = neighbour / gridSideLength;
						int col = neighbour % gridSideLength;

						for (int tmp = 0; tmp < progress; ++tmp) printf(" ");
						printf("neighbour character: %c at %d\n", grid[row][col], neighbour);

						if (grid[row][col] != sequence[progress + 1])
							return false;

						// check if index is for the last value
						if (progress + 1 >= SEQUENCE_LENGTH - 1)
							return true;

						int neighbourValidDirections = Direction::FindValidDirections(neighbour);
						if (!Direction::IsValidDirection(neighbourValidDirections, direction))
							return false;

						return FindRestOfSequence(Direction::GetNeighbour(neighbour, direction), direction, progress + 1);
					};
				total += FindRestOfSequence(neighbourIndex, directionToCheck, 0) ? 1 : 0;
			}
		}

		printf("\n");
	}

	printf("Total: %d\n", total);
	return 0;
}

int main(int argc, char* argv[])
{
	return main_01(argc, argv);
}
