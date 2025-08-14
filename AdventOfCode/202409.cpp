#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define PRINT(x) std::cout << #x << ": " << (x) << "\n"

/// @brief A string of digits representing files and empty blocks.
using DiskMap = std::string;

/// @brief The contents of a block are represented by IDs.
using Block = int;

/// @brief The ID used by free blocks.
constexpr Block freeBlockID = -1;

/// @brief A type used to represent very large integers.
using BigNumber = unsigned long long;

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
/// @param diskMap The disk map stated in the input file.
/// @return true on success, false otherwise.
bool ReadDataFromFile(const std::string& filename, DiskMap& diskMap)
{
	std::fstream file(filename);
	if (!file)
	{
		printf("sorry %s isn't a file\n", filename.c_str());
		return false;
	}

	std::getline(file, diskMap); // nice :)

	return true;
}

/// @brief Constructs blocks of free space and files using a given disk map.
/// @param diskMap The disk map used to build the blocks of files and free space.
/// @return A vector of blocks and empty space, according to the given disk map.
std::vector<Block> BuildBlocks(const DiskMap& diskMap)
{
	std::vector<Block> blocks;
	Block blockID = 0;

	for (unsigned i = 0; i < diskMap.size(); ++i)
	{
		int size = diskMap[i] - '0';

		while (size--)
			blocks.emplace_back(i % 2 ? freeBlockID : blockID);

		// get ready for next file (which has a new ID = ID + 1)
		if (i % 2)
			++blockID;
	}

	blocks.shrink_to_fit();

	return blocks;
}

/// @brief Moves blocks to leftmost free spaces until none are left.
/// @param blocks The blocks to modify.
void MoveBlocks(std::vector<Block>& blocks)
{
	auto freeBlockIt = std::find(blocks.begin(), blocks.end(), freeBlockID);

	// no free blocks, no point
	if (freeBlockIt == blocks.end())
		return;

	// reverse search + casting reverse iterators are a pain :)
	// let's just assume the last character is always a non-empty block :)
	auto lastBlockIt = blocks.end() - 1;

	while (freeBlockIt < lastBlockIt)
	{
		// swap blocks
		std::swap(*freeBlockIt, *lastBlockIt);

		// update iterators
		do { ++freeBlockIt; } while (*freeBlockIt != freeBlockID);
		do { --lastBlockIt; } while (*lastBlockIt == freeBlockID);
	}
}

/// @brief Calculates the checksum of a given series of blocks.
/// @param blocks The blocks to calculate the checksum for.
/// @return The checksum from the given blocks.
BigNumber ComputeChecksum(const std::vector<Block>& blocks)
{
	BigNumber checksum = 0;
	for (unsigned i = 0; i < blocks.size(); ++i)
		checksum += (blocks[i] == freeBlockID ? 0 : i * blocks[i]);
	return checksum;
}

/// @brief Reads in a diskmap from a file, moves blocks to empty spaces and computes the checksum.
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

	DiskMap diskMap;
	if (!ReadDataFromFile(filename, diskMap))
		return -1;

	std::vector<Block> blocks = BuildBlocks(diskMap);

	MoveBlocks(blocks);

	BigNumber checksum = ComputeChecksum(blocks);

	PRINT(checksum);

	return 0;
}
