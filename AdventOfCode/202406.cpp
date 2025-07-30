#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define PRINT(x) std::cout << #x << ": " << (x) << "\n"

template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec)
{
	stream << '[';
	for (unsigned i = 0; i < vec.size(); ++i)
		stream << vec[i] << ((i < vec.size() - 1) ? "," : "");
	return stream << ']';
}

// C++ scoped enums but not really (https://en.cppreference.com/w/cpp/language/enum.html#Example)
struct Symbols
{
	using Type = int;

	static const Type
		EMPTY = '.',
		OBSTRUCTION = '#',
		START = '^',
		VISITED = 'X',

		VISITED_N = 0b0001,
		VISITED_S = 0b0010,
		VISITED_E = 0b0100,
		VISITED_W = 0b1000;
};

enum class Direction { N, S, E, W };

Direction TurnRight(Direction dir)
{
	switch (dir)
	{
	case Direction::N: return Direction::E;
	case Direction::S: return Direction::W;
	case Direction::E: return Direction::S;
	case Direction::W: return Direction::N;
	default: return Direction::N;
	}
}

Symbols::Type VisitedDirection(Direction dir)
{
	switch (dir)
	{
	case Direction::N: return Symbols::VISITED_N;
	case Direction::S: return Symbols::VISITED_S;
	case Direction::E: return Symbols::VISITED_E;
	case Direction::W: return Symbols::VISITED_W;
	default: return Symbols::VISITED_N;
	}
}

struct Position
{
	unsigned row, col;

	static unsigned gridSideLength;

	bool IsInGrid()
	{
		return row < gridSideLength && col < gridSideLength;
	}

	void Advance(Direction dir)
	{
		switch (dir)
		{
		case Direction::N: --row; return;
		case Direction::S: ++row; return;
		case Direction::E: ++col; return;
		case Direction::W: --col; return;
		default: return;
		}
	}
};

unsigned Position::gridSideLength = 0;

inline std::ostream& operator<<(std::ostream& stream, const Position& pos)
{
	return stream << '(' << pos.row << ',' << pos.col << ')';
}

// oops, 50 min rabbit hole :)
unsigned GetFileLineCount(const char* filename)
{
	// Getting a file's line count without `std::getline`-ing all the time! Thanks to these links!
	// https://stackoverflow.com/a/3482093/30412497
	// https://www.reddit.com/r/cpp_questions/comments/11wlf49/whats_the_most_efficient_way_to_get_the_line/

	/*
	The only thing you've left to do is determine if the file is empty or not, and whether the last line
	ended with a newline or not. Just seekg 1 from the end and peek to see if the last character is a
	newline. If not, you have to add a +1 to your count.
	*/
	std::fstream file(filename);
	return std::count_if(std::istreambuf_iterator<char>{file}, {}, [](char c) { return c == '\n'; });
}

using Grid = std::vector<std::string>;

bool ReadDataFromFile(const std::string& filename, Grid& grid)
{
	std::fstream file(filename);
	if (!file)
	{
		printf("sorry %s isn't a file\n", filename.c_str());
		return false;
	}

	std::string line;
	while (std::getline(file, line))
		grid.emplace_back(line);

	grid.shrink_to_fit();
	Position::gridSideLength = grid.size();
	return true;
}

Position FindAndClearStartPosition(Grid& grid)
{
	Position result;
	for (unsigned i = 0; i < grid.size(); ++i)
	{
		for (unsigned j = 0; j < grid[i].size(); ++j)
		{
			if (grid[i][j] == Symbols::START)
			{
				result = Position{ .row = i, .col = j };
				grid[i][j] = Symbols::EMPTY;
			}
		}
	}
	return result;
}

int CountVisitedPositions(Grid& grid)
{
	Position startPos = FindAndClearStartPosition(grid);

	Position currentPosition = startPos;
	Direction currentDirection = Direction::N;

	while (currentPosition.IsInGrid())
	{
		Position check = currentPosition;
		check.Advance(currentDirection);

		bool wasObstructed = false;
		if (check.IsInGrid())
		{
			if (grid[check.row][check.col] == Symbols::OBSTRUCTION)
			{
				wasObstructed = true;
				currentDirection = TurnRight(currentDirection);
			}
		}

		if (!wasObstructed)
		{
			grid[currentPosition.row][currentPosition.col] = Symbols::VISITED;
			currentPosition.Advance(currentDirection);
		}
	}

	unsigned total = 0;
	for (unsigned i = 0; i < grid.size(); ++i)
		for (unsigned j = 0; j < grid[i].size(); ++j)
			if (grid[i][j] == Symbols::VISITED)
				++total;

	return total;
}

int CountPossibleObstacles(const Grid& grid)
{
	// for each possible obstacle position
	//   put the obstacle in the board
	//   let the guard run
	//     if the guard runs over the same place twice *while going in the same direction*
	//       count guard as stuck (?)

	unsigned total = 0;
	for (unsigned i = 0; i < grid.size(); ++i)
	{
		for (unsigned j = 0; j < grid[i].size(); ++j)
		{
			if (grid[i][j] == Symbols::OBSTRUCTION || grid[i][j] == Symbols::START)
				continue;

			// copy over grid and add one obstacle to it
			Grid gridWithObstacle{ grid };
			gridWithObstacle[i][j] = Symbols::OBSTRUCTION;

			bool loopFound = false;

			Position currentPosition = FindAndClearStartPosition(gridWithObstacle);
			Direction currentDirection = Direction::N;

			while (currentPosition.IsInGrid() && !loopFound)
			{
				Position check = currentPosition;
				check.Advance(currentDirection);

				bool wasObstructed = false;
				if (check.IsInGrid())
				{
					if (gridWithObstacle[check.row][check.col] == Symbols::OBSTRUCTION)
					{
						wasObstructed = true;
						currentDirection = TurnRight(currentDirection);
					}
				}

				if (!wasObstructed)
				{
					char& currentSymbol = gridWithObstacle[currentPosition.row][currentPosition.col];
					Symbols::Type newSymbol = VisitedDirection(currentDirection);

					if (currentSymbol == Symbols::EMPTY)
					{
						currentSymbol = newSymbol;
					}
					else if (currentSymbol & newSymbol)
					{
						std::cout << "WARNING: RE-TREADING SAME DIRECTION AT " << currentPosition << '\n';
						loopFound = true;
						++total;
					}
					else
					{
						currentSymbol |= newSymbol; // bitwise, to overlay directions on top of each other
					}

					currentPosition.Advance(currentDirection);
				}
			}
		}
	}
	return total;
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

	// int total = CountVisitedPositions(grid); // part 1
	int total = CountPossibleObstacles(grid); // part 2
	PRINT(total);

	return 0;
}
