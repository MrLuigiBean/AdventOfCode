#include <iostream>
#include <fstream>

int main_01(int argc, char* argv[])
{
	const char* defaultFilename = "small.txt";
	if (argc != 2)
	{
		printf("Usage: ./this_program_name some_input_file.txt\n");
		// return -1;
		printf("Using default filename %s...\n", defaultFilename);
	}

	std::fstream file(argc == 2 ? argv[1] : defaultFilename);
	if (!file)
	{
		printf("sorry %s isn't a file\n", argv[1]);
		return -1;
	}

	return 0;
}

int main_02(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	printf("Hello 2!\n");
	return 0;
}

int main(int argc, char* argv[])
{
	main_01(argc, argv);
	// main_02(argc, argv);
	return 0;
}
