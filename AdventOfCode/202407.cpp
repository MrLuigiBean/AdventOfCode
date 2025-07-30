#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
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

enum class Operator { ADD, MULTIPLY };

inline std::ostream& operator<<(std::ostream& stream, const Operator& op)
{
	return stream << (op == Operator::ADD ? '+' : '*');
}

using BigNumber = unsigned long long;

struct Equation
{
	BigNumber testValue = 0;
	std::vector<unsigned> numbers;
	bool isTrueEquation = false;

	Equation(const std::string& str)
	{
		std::istringstream sstream{ str };
		char temp;
		sstream >> testValue >> temp;

		int number;
		while (sstream >> number)
			numbers.emplace_back(number);

		numbers.shrink_to_fit();
	}

	BigNumber ComputeEquation(const std::vector<Operator>& operators) const
	{
		BigNumber total = numbers.front();
		for (unsigned i = 0; i < numbers.size() - 1; ++i)
		{
			Operator op = operators[i];
			BigNumber number = numbers[i + 1];

			switch (op)
			{
			case Operator::ADD:      total += number; break;
			case Operator::MULTIPLY: total *= number; break;
			default: break;
			}
		}
		return total;
	}
};

inline std::ostream& operator<<(std::ostream& stream, const Equation& eqn)
{
	// return stream << "testValue: " << eqn.testValue << ", numbers: " << eqn.numbers;
	stream << eqn.testValue << ": ";
	stream << eqn.numbers[0];
	for (unsigned i = 1; i < eqn.numbers.size(); ++i)
		stream << ' ' << eqn.numbers[i];
	return stream;
}

void FlagTrueEquations(std::vector<Equation>& equations)
{
	for (Equation& equation : equations)
	{
		std::vector<Operator> operators(equation.numbers.size() - 1);

		BigNumber totalCombinations = 1 << operators.size();
		for (BigNumber currentCombination = 0; currentCombination < totalCombinations; ++currentCombination)
		{
			for (unsigned i = 0; i < operators.size(); ++i)
			{
				unsigned operatorState = (1 << i) & currentCombination;

				// index from the back to match the bits of `currentCombination`
				unsigned index = operators.size() - 1 - i;

				operators[index] = operatorState ? Operator::MULTIPLY : Operator::ADD;
			}

			if (equation.ComputeEquation(operators) == equation.testValue)
			{
				PRINT(equation.testValue);
				PRINT(equation.numbers);
				PRINT(operators);
				equation.isTrueEquation = true;
				break;
			}
		}
	}
}

bool ReadDataFromFile(const std::string& filename, std::vector<Equation>& equations)
{
	std::fstream file(filename);
	if (!file)
	{
		printf("sorry %s isn't a file\n", filename.c_str());
		return false;
	}

	std::string line;
	while (std::getline(file, line))
		equations.emplace_back(line);

	equations.shrink_to_fit();
	return true;
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

	std::vector<Equation> equations;

	if (!ReadDataFromFile(filename, equations))
		return -1;

	FlagTrueEquations(equations);

	BigNumber total = 0;

	auto newEnd = std::remove_if(equations.begin(), equations.end(), [](const Equation& eqn)
		{ return !eqn.isTrueEquation; });

	for (auto it = equations.begin(); it < newEnd; ++it)
		total += it->testValue;

	PRINT(total);

	return 0;
}
