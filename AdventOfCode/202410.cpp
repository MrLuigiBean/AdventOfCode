#include <fstream>
#include <iostream>
#include <set>
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

	/// @brief Checks the equality of this object and the other.
	/// @param other The other object to compare to.
	/// @return true if this object's row and col are the same as the other's.
	inline bool operator==(const Idx2D& other)
	{
		return col == other.col && row == other.row;
	}

	/// @brief Compares two objects using their row and col.
	/// @param other The other object to compare to.
	/// @return true if this object's row is lesser than the other. When tied, col is compared.
	inline bool operator<(const Idx2D& other) const
	{
		// if (row == other.row)
		// 	return col < other.col;
		// else
		// 	return row < other.row;

		// ^ same as this v
		return (row == other.row) ? (col < other.col) : (row < other.row);
	}

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

/// @brief Alias for Idx2D
using Index2d = Idx2D;

/// @brief Alias for Idx2D
using Coord = Idx2D;

/// @brief Represents the four cardinal directions.
struct Directions { enum { NORTH, WEST, EAST, SOUTH, TOTAL }; };

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

#pragma region Part 1

/// @brief Recursively iterates through the height data to find complete trails.
/// @param stepHeights The grid of height data.
/// @param endPointsReached The set of all unique end points reached by this trail.
/// @param currentPoint The current point being considered.
void CalculateTrailheadScoreRecursive(const StepHeights& stepHeights,
	std::set<Idx2D>& endPointsReached, const Idx2D& currentPoint)
{
	int currentHeight = stepHeights[currentPoint.row][currentPoint.col];

	if (currentHeight == 9)
	{
		endPointsReached.insert(currentPoint);
		return;
	}

	Idx2D neighbours[Directions::TOTAL]{};
	int neighbourSize = 0;
	for (int i = 0; i < Directions::TOTAL; ++i)
	{
		Idx2D::IfArgs ifArgs;
		switch (i)
		{
		case Directions::NORTH: ifArgs.modRow = -1; break;
		case Directions::WEST:  ifArgs.modCol = -1; break;
		case Directions::EAST:  ifArgs.modCol = +1; break;
		case Directions::SOUTH: ifArgs.modRow = +1; break;
		default: break;
		}

		Idx2D neighbour = currentPoint.If(ifArgs);

		// add the neighbour if its valid and its height is the next in the sequence

		bool isCoordValid = 0 <= neighbour.row && neighbour.row <= static_cast<int>(stepHeights.size()) - 1 &&
			0 <= neighbour.col && neighbour.col <= static_cast<int>(stepHeights.front().size()) - 1;

		if (isCoordValid)
		{
			int neighbourHeight = stepHeights[neighbour.row][neighbour.col];

			if (neighbourHeight == currentHeight + 1)
				neighbours[neighbourSize++] = neighbour;
		}
	}

	for (int i = 0; i < neighbourSize; ++i)
		CalculateTrailheadScoreRecursive(stepHeights, endPointsReached, neighbours[i]);
}

/// @brief Calculates the score of a trailhead.
/// @param stepHeights The grid of height data.
/// @param startPoint The beginning of the trailhead.
/// @return The trailhead's score.
int CalculateTrailheadScore(const StepHeights& stepHeights, const Idx2D& startPoint)
{
	std::set<Idx2D> endPointsReached;

	CalculateTrailheadScoreRecursive(stepHeights, endPointsReached, startPoint);

	return endPointsReached.size();
}

/// @brief Calculates the total sum of all trailhead scores.
/// @param stepHeights The grid of height data.
/// @return The sum of all trailhead scores.
int CalculateCombinedTrailheadScore(const StepHeights& stepHeights)
{
	std::vector<Idx2D> startPoints; // initializing trailheads
	for (unsigned i = 0; i < stepHeights.size(); ++i)
	{
		for (unsigned j = 0; j < stepHeights[i].size(); ++j)
		{
			if (stepHeights[i][j] == 0)
				startPoints.emplace_back(i, j);
		}
	}
	startPoints.shrink_to_fit();

	int totalScore = 0;

	for (const auto& startPoint : startPoints)
		totalScore += CalculateTrailheadScore(stepHeights, startPoint);

	return totalScore;
}

/// @brief This program computes the total trailhead score from a file with height data.
int main_01(int argc, char* argv[])
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

	int total = CalculateCombinedTrailheadScore(stepHeights);

	PRINT(total);

	return 0;
}

#pragma endregion

#pragma region Part 2

void CalculateTrailheadRatingRecursive(const StepHeights& stepHeights,
	std::set<Idx2D>& endPointsReached, const Idx2D& currentPoint)
{
	int currentHeight = stepHeights[currentPoint.row][currentPoint.col];

	if (currentHeight == 9)
	{
		endPointsReached.insert(currentPoint);
		return;
	}

	Idx2D neighbours[Directions::TOTAL]{};
	int neighbourSize = 0;
	for (int i = 0; i < Directions::TOTAL; ++i)
	{
		Idx2D::IfArgs ifArgs;
		switch (i)
		{
		case Directions::NORTH: ifArgs.modRow = -1; break;
		case Directions::WEST:  ifArgs.modCol = -1; break;
		case Directions::EAST:  ifArgs.modCol = +1; break;
		case Directions::SOUTH: ifArgs.modRow = +1; break;
		default: break;
		}

		Idx2D neighbour = currentPoint.If(ifArgs);

		// add the neighbour if its valid and its height is the next in the sequence

		bool isCoordValid = 0 <= neighbour.row && neighbour.row <= static_cast<int>(stepHeights.size()) - 1 &&
			0 <= neighbour.col && neighbour.col <= static_cast<int>(stepHeights.front().size()) - 1;

		if (isCoordValid)
		{
			int neighbourHeight = stepHeights[neighbour.row][neighbour.col];

			if (neighbourHeight == currentHeight + 1)
				neighbours[neighbourSize++] = neighbour;
		}
	}

	for (int i = 0; i < neighbourSize; ++i)
		CalculateTrailheadRatingRecursive(stepHeights, uniqueTrails, neighbours[i]);
}

/// @brief Calculates the rating of a trailhead.
/// @param stepHeights The grid of height data.
/// @param startPoint The beginning of the trailhead.
/// @return The trailhead's rating.
int CalculateTrailheadRating(const StepHeights& stepHeights, const Idx2D& startPoint)
{
	std::set<Idx2D> uniqueTrails;

	CalculateTrailheadRatingRecursive(stepHeights, uniqueTrails, startPoint);

	return uniqueTrails.size();
}

/// @brief Calculates the total sum of all trailhead ratings.
/// @param stepHeights The grid of height data.
/// @return The sum of all trailhead ratings.
int CalculateCombinedTrailheadRating(const StepHeights& stepHeights)
{
	std::vector<Idx2D> startPoints; // initializing trailheads
	for (unsigned i = 0; i < stepHeights.size(); ++i)
	{
		for (unsigned j = 0; j < stepHeights[i].size(); ++j)
		{
			if (stepHeights[i][j] == 0)
				startPoints.emplace_back(i, j);
		}
	}
	startPoints.shrink_to_fit();

	int totalRatings = 0;

	for (const auto& startPoint : startPoints)
		totalRatings += CalculateTrailheadRating(stepHeights, startPoint);

	return totalRatings;
}

/// @brief This program computes the total trailhead rating from a file with height data.
int main_02(int argc, char* argv[])
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

	int total = CalculateCombinedTrailheadRating(stepHeights);

	PRINT(total);

	return 0;
}

#pragma endregion

int main(int argc, char* argv[])
{
	// return main_01(argc, argv);
	return main_02(argc, argv);
}
