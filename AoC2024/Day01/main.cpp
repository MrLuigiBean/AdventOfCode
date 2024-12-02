// Day 01!
#define SOL_1_1 1
#define SOL_1_2 1

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <map>

#if SOL_1_1
int main()
{
	std::fstream file("input.txt", std::ios_base::in);

	unsigned long int num = 0;
	std::multiset<int> set1;
	std::multiset<int> set2;

	printf("Getting numbers...\n");

	while (file.good())
	{
		file >> num;
		set1.emplace(num);

		file >> num;
		set2.emplace(num);
	}

	std::vector<int> v1(set1.begin(), set1.end());
	std::vector<int> v2(set2.begin(), set2.end());

	int result = 0;

	printf("Calculating difference...\n");
	for (int i = 0; i < v1.size(); ++i)
	{
		int diff = v1[i] - v2[i];
		if (diff < 0)
			diff *= -1;
		result += diff;
	}

	printf("result: %d\n", result);

	return 0;
}
#endif

#if SOL_1_2
int main()
{
	std::fstream file("input_l.txt", std::ios_base::in);

	std::vector<int> v1;
	v1.reserve(1000);
	std::vector<int> v2;
	v2.reserve(1000);

	printf("Getting numbers...\n");
	int num = 0;
	while (file.good())
	{
		file >> num;
		v1.emplace_back(num);

		file >> num;
		v2.emplace_back(num);
	}

	printf("Constructing frequency map...\n");
	std::map<int, int> v2FreqMap;
	for (int i = 0; i < v2.size(); ++i)
		++v2FreqMap[v2[i]];

	printf("Calculating result...\n");
	unsigned long int result = 0;
	for (int i = 0; i < v1.size(); ++i)
	{
		int freq = 0;
		try
		{
			freq = v2FreqMap.at(v1[i]);
		}
		catch (...)
		{
		}
		result += v1[i] * freq;
	}

	printf("result: %zu\n", result);

	return 0;
}
#endif

int main()
{
	printf("hello!\n");
	return 0;
}
