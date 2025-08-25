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

/// @brief A type used to represent very large integers.
using BigNumber = unsigned long long;

/// @brief Determines the number of digits an integer has.
/// @param number The integer to query.
/// @return The number of digits in the given integer.
int NumberOfDigits(BigNumber number)
{
	int digits = 0;
	do { ++digits; } while (number /= 10);
	return digits;
}

/// @brief Computes base ^ power.
/// @param base The base to use in exponentiation.
/// @param power The exponent to raise base to.
/// @return Returns base ^ power.
BigNumber Pow(BigNumber base, BigNumber power)
{
	BigNumber result = 1;
	while (power--)
		result *= base;
	return result;
}

/// @brief Follows three rules on a line of rocks with numbers.
/// @param numbers The numbers written on the line of rocks.
/// @param iterations The number of 'blinks' that happen.
void Iterate(std::vector<BigNumber>& numbers, int iterations)
{
	struct Insertion
	{
		BigNumber pos, number;
		Insertion(BigNumber pos_, BigNumber number_) : pos{ pos_ }, number{ number_ } {}
	};

	std::vector<Insertion> insertions; // save insertions for after this loop completes

	while (iterations--)
	{
		insertions.clear();

		for (unsigned i = 0; i < numbers.size(); ++i)
		{
			// rule 1: zeroes become ones
			if (numbers[i] == 0)
			{
				numbers[i] = 1;
				continue;
			}

			// rule 2: split number when digits are even
			if (int digits = NumberOfDigits(numbers[i]); digits % 2 == 0)
			{
				BigNumber secondHalf = 0;
				for (int secondHalfDigit = 0; secondHalfDigit < (digits / 2); ++secondHalfDigit)
				{
					int digit = numbers[i] % 10;
					secondHalf += digit * Pow(10, secondHalfDigit);
					numbers[i] /= 10;
				}

				// at the end, numbers[i] should have the value of firstHalf! :D

				insertions.emplace_back(i + 1, secondHalf);

				continue;
			}

			// default rule
			numbers[i] *= 2024;
		}

		// do insertions here (in reverse!!!)
		for (int i = insertions.size() - 1; i >= 0; --i)
			numbers.insert(numbers.begin() + insertions[i].pos, insertions[i].number);
	}
}

/// @brief Reads and numbers from the file.
/// @param filename The name of the file to obtain data from.
/// @param numbers The numbers read in from the file.
/// @return true on success, false otherwise.
bool ReadDataFromFile(const std::string& filename, std::vector<BigNumber>& numbers)
{
	std::fstream file(filename);
	if (!file)
	{
		printf("sorry %s isn't a file\n", filename.c_str());
		return false;
	}

	BigNumber number;
	while (file >> number)
		numbers.emplace_back(number);

	return true;
}

/// @brief Calculates the number of rocks after blinking.
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

	std::vector<BigNumber> numbers;
	if (!ReadDataFromFile(filename, numbers))
		return -1;

	Iterate(numbers, 25);

	PRINT(numbers.size());

	return 0;
}
