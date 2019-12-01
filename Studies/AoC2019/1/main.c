/* Advent of code 2019 - 1 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	/* Check comand line */
	if (argc < 2)
	{
		puts("Please provide input file path");
		return 1;
	}

	/* Input parse init */
	char* path = argv[1];
	char line_buffer[1024];
	FILE* input_file = fopen(path, "r");

	/* Check the read to be succefull */
	if (input_file == NULL)
	{
		printf("Error reading file %s\n", path);
		return 1;
	}

	long int total = 0;

	/* Cicle lines */
	while(fgets(line_buffer, 1024, input_file)) 
	{
		/* Get the mass */
		int mass = atoi(line_buffer);

		/*
		total += mass/3-2;
		*/		

		/* Compute the needed fuel */
		int fuel = 0;
		int needed = mass/3-2;
		while (needed > 0) 
		{
			fuel += needed;
			needed = needed/3-2;
		}
		
		/* Add to total */
		total += fuel;
	}

	/* Print result */
	printf("%ld\n", total);

	return 0;
}

