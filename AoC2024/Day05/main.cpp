#include <iostream>

int main_01(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	printf("Hello 1!\n");
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
