#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define PRINT(x) std::cout << #x << ": " << (x) << "\n"

/// @brief Activates Part 1's solution.
constexpr bool isPart1 = false;

/// @brief Activates Part 2's solution.
constexpr bool isPart2 = true;

/// @brief A string of digits representing files and empty blocks.
using DiskMap = std::string;

/// @brief The contents of a block are represented by IDs.
using Block = int;

/// @brief The ID used by free blocks.
constexpr Block freeBlockID = -1;

/// @brief A type used to represent very large integers.
using BigNumber = unsigned long long;

/// @brief Represents a span of file blocks in a DiskMap.
struct FileSpan
{
	/// @brief The ID of a file.
	int id = 0;

	/// @brief The index of the starting block of this file in the DiskMap.
	int offset = 0;

	/// @brief The number of blocks this file has.
	int size = 0;
};

/// @brief Represents a span of free space blocks in a DiskMap.
struct FreeSpaceSpan
{
	/// @brief The index of the starting block of this free space in the DiskMap.
	int offset = 0;

	/// @brief The number of blocks this free space has.
	int size = 0;
};

/// @brief The lists created by GenerateDiskMapLists().
struct DiskMapLists
{
	/// @brief A list of files in the given DiskMap.
	std::vector<FileSpan> files;

	/// @brief A list of free spaces in the given DiskMap.
	std::vector<FreeSpaceSpan> freeSpaces;
}; // structured binding wahoo!

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

/// @brief Generates the file and free space lists from a given disk map.
/// @param diskMap The disk map to create the lists from.
/// @return (Structured binding is recommended.) The lists generated from the disk map.
DiskMapLists GenerateDiskMapLists(const DiskMap& diskMap)
{
	std::vector<FreeSpaceSpan> freeSpaces;
	std::vector<FileSpan> files;
	bool isFile = true;
	int offset = 0;
	int id = 0;

	for (const auto ch : diskMap)
	{
		int size = ch - '0';

		if (isFile)
			files.emplace_back(FileSpan{ id++, offset, size });
		else
			freeSpaces.emplace_back(FreeSpaceSpan{ offset, size });

		offset += size;

		// prepare for next iteration
		isFile = !isFile;
	}

	return { files,  freeSpaces };
}

/// @brief Moves files in a disjk towards the left-most freespace available.
/// @param files The list representing files to move in the disk.
/// @param freeSpaces The list representing available free spaces in the disk.
void MoveFiles(std::vector<FileSpan>& files, std::vector<FreeSpaceSpan>& freeSpaces)
{
	for (int fileIndex = files.size() - 1; fileIndex >= 0; --fileIndex)
	{
		FileSpan& file = files[fileIndex];

		for (unsigned freeIndex = 0; freeIndex < freeSpaces.size(); ++freeIndex)
		{
			FreeSpaceSpan& freeSpace = freeSpaces[freeIndex];

			// ignore free spaces that are not big enough
			if (freeSpace.size < file.size)
				continue;

			// don't bother if the freespace comes later than the file
			if (file.offset < freeSpace.offset)
				continue;

			// 'move' the file to the free space
			file.offset = freeSpace.offset;

			// update the free space
			freeSpace.offset += file.size;
			freeSpace.size -= file.size;

			// only one movement - we're done here
			break;
		}
	}
}

/// @brief Calculates the checksum of a given series of files.
/// @param files The list representing files in the disk
/// @return The checksum computed from the file on disk.
BigNumber ComputeChecksum(const std::vector<FileSpan>& files)
{
	BigNumber total = 0;
	for (const auto& file : files)
	{
		BigNumber miniSum = 0;
		for (int i = 0; i < file.size; ++i)
			miniSum += file.offset + i;
		total += file.id * miniSum;
	}
	return total;
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

	if (isPart1 && isPart2)
	{
		printf("Both Part 1 and Part 2 are active - please choose only one.\n");
		return -1;
	}

	DiskMap diskMap;
	if (!ReadDataFromFile(filename, diskMap))
		return -1;

	if (isPart1)
	{
		std::vector<Block> blocks = BuildBlocks(diskMap);

		MoveBlocks(blocks);

		BigNumber checksum = ComputeChecksum(blocks);

		PRINT(checksum);
	}
	else if (isPart2)
	{
		auto [fileList, freeSpaceList](GenerateDiskMapLists(diskMap));

		MoveFiles(fileList, freeSpaceList);

		std::sort(fileList.begin(), fileList.end(),
			[](FileSpan& a, FileSpan& b) { return a.offset < b.offset; });

		BigNumber checksum = ComputeChecksum(fileList);

		PRINT(checksum);
	}

	return 0;
}
