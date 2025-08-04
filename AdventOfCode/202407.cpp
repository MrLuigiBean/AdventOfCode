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

enum class Operator { ADD, MULTIPLY, CONCAT };
constexpr static unsigned numOfOperatorTypes = 3;

inline std::ostream& operator<<(std::ostream& stream, const Operator& op)
{
	const char* ch = "";
	switch (op)
	{
	case Operator::ADD:      ch = "+";  break;
	case Operator::MULTIPLY: ch = "*";  break;
	case Operator::CONCAT:   ch = "||"; break;
	default:                            break;
	}
	return stream << ch;
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
			case Operator::CONCAT:
				total = std::stoull(std::to_string(total) + std::to_string(number));
				break;
			default: break;
			}
		}
		return total;
	}
};

inline std::ostream& operator<<(std::ostream& stream, const Equation& eqn)
{
	stream << eqn.testValue << ": ";
	stream << eqn.numbers[0];
	for (unsigned i = 1; i < eqn.numbers.size(); ++i)
		stream << ' ' << eqn.numbers[i];
	return stream;
}

void OldFlagTrueEquations(std::vector<Equation>& equations)
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

static constexpr BigNumber Pow(unsigned base, unsigned power)
{
	BigNumber total = 1;
	while (power--)
		total *= base;
	return total;
}

static void GenerateOperatorsFromCombination(std::vector<Operator>& ops, BigNumber combination)
{
	// Since there are 3 operators available (numOfOperatorTypes = 3),
	// place the corresponding operators in the array using the
	// ("hundreds", "tens", "ones" places in base 3) in `currentCombination`.

	// e.g. If, currentCombination = 4, ops.size() = 3, then:
	// ops[0] = the value of "nine's" place in 4 = 0
	// ops[0] = the value of "three's" place in 4 = 1
	// ops[1] = the value of ones place in 4 = 1

	for (unsigned i = 0; i < ops.size(); ++i)
	{
		auto n_th_place = ops.size() - 1 - i;
		auto op = (combination / Pow(numOfOperatorTypes, n_th_place)) % numOfOperatorTypes;
		ops[i] = static_cast<Operator>(op);
	}
}

void FlagTrueEquations(std::vector<Equation>& equations)
{
	for (Equation& equation : equations)
	{
		std::vector<Operator> ops(equation.numbers.size() - 1);

		BigNumber totalCombinations = Pow(numOfOperatorTypes, ops.size());

		for (BigNumber currentCombination = 0; currentCombination < totalCombinations; ++currentCombination)
		{
			GenerateOperatorsFromCombination(ops, currentCombination);

			BigNumber total = equation.numbers.front();

			for (unsigned i = 0; i < ops.size(); ++i)
			{
				unsigned number = equation.numbers[i + 1];
				switch (ops[i])
				{
				case Operator::ADD:      total += number; break;
				case Operator::MULTIPLY: total *= number; break;
				case Operator::CONCAT:   total = std::stoull(std::to_string(total) + std::to_string(number)); break;
				default:                 break;
				}

				// since all operators only increase the value, stop this sequence
				// of operators if it already exceeds the expected total
				if (total > equation.testValue)
					break;
			}

			// if a match is found, great. no need to keep trying out more combinations.
			if (total == equation.testValue)
			{
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
