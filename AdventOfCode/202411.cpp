#include <forward_list>
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

using Container = std::forward_list<BigNumber>;

inline std::ostream& operator<<(std::ostream& stream, const Container& list)
{
	stream << '[';
	for (Container::const_iterator it = list.begin(); it != list.end(); ++it)
		stream << *it << (std::next(it) != list.end() ? "," : "");
	return stream << ']';
}

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
void Iterate(Container& numbers, int iterations)
{
	struct Insertion
	{
		Container::iterator pos;
		BigNumber number;
		Insertion(Container::iterator pos_, BigNumber number_) : pos{ pos_ }, number{ number_ } {}
	};

	std::vector<Insertion> insertions; // save insertions for after this loop completes

	while (iterations--)
	{
		PRINT(iterations);

		// PRINT(numbers);

		insertions.clear();

		for (Container::iterator it = numbers.begin(); it != numbers.end(); ++it)
		{
			BigNumber& number = *it;

			// rule 1: zeroes become ones
			if (number == 0)
			{
				number = 1;
				continue;
			}

			// rule 2: split number when digits are even
			if (int digits = NumberOfDigits(number); digits % 2 == 0)
			{
				BigNumber secondHalf = 0;
				for (int secondHalfDigit = 0; secondHalfDigit < (digits / 2); ++secondHalfDigit)
				{
					int digit = number % 10;
					secondHalf += digit * Pow(10, secondHalfDigit);
					number /= 10;
				}

				// at the end, numbers[i] should have the value of firstHalf! :D

				insertions.emplace_back(it, secondHalf);

				continue;
			}

			// default rule
			number *= 2024;
		}

		for (const auto& item : insertions)
			numbers.insert_after(item.pos, item.number);
	}
}

/// @brief Reads and numbers from the file.
/// @param filename The name of the file to obtain data from.
/// @param numbers The numbers read in from the file.
/// @return true on success, false otherwise.
bool ReadDataFromFile(const std::string& filename, Container& numbers)
{
	std::fstream file(filename);
	if (!file)
	{
		printf("sorry %s isn't a file\n", filename.c_str());
		return false;
	}

	BigNumber number;
	std::vector<BigNumber> temp;
	while (file >> number)
		temp.emplace_back(number);

	numbers = Container(temp.begin(), temp.end());

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

	Container numbers;
	if (!ReadDataFromFile(filename, numbers))
		return -1;

	Iterate(numbers, 75);

	// PRINT(numbers);

	BigNumber size = 0;
	for (Container::iterator it = numbers.begin(); it != numbers.end(); ++it)
		++size;

	PRINT(size);

	return 0;
}
