/* Advent of code 2019 - 2 */
#define CHECK 19690720

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
	FILE* input_file = fopen(path, "r");

	/* Check the read to be succefull */
	if (input_file == NULL)
	{
		printf("Error reading file %s\n", path);
		return 1;
	}

	int val_count = 1;
	char c;

	/* Count commas */
	while ((c = fgetc(input_file)) != EOF)
	{
		if (c == ',')
			val_count++;
	}

	/* Rewind file and allocate array and buffer*/
	rewind(input_file);
	int* file = malloc(val_count*sizeof(int));
	int* mem = malloc(val_count*sizeof(int));
	char* buffer = malloc(val_count*16);

	/* Parse the file */
	fgets(buffer, val_count*16, input_file);
	int val;
	
	for (int i = 0; i < val_count; i++)
	{
		sscanf(buffer, "%d,%s", &val, buffer);
		file[i] = val;
	}

	fclose(input_file);
	free(buffer);
	
	/* Emulator */
	for (int noun = 0; noun < 100; noun++)
	{
		for (int verb = 0; verb < 100; verb++)
		{

			/* Reset machine state */
			for (int i = 0; i < val_count; i++)
				mem[i] = file[i];

			mem[1] = noun;
			mem[2] = verb;

			/* Execute loop */
			for(int pc = 0; mem[pc] != 99; pc += 4) 
			{
				/* Exit */
				if (mem[pc] == 99)
					break;
			
				/* Add */
				if (mem[pc] == 1)
					mem[mem[pc+3]] = mem[mem[pc+1]] + mem[mem[pc+2]];
			
				/* Mul */
				else
					mem[mem[pc+3]] = mem[mem[pc+1]] * mem[mem[pc+2]];
			}

			/* Check result and print */
			if (mem[0] == CHECK)
			{
				printf("%d\n", 100*noun+verb);
				free(file);
				free(mem);
				return 0;			
			}
		}
	}
	
	/* We found nothing? */
	puts("Error, no value found!");
	free(file);
	free(mem);
	return 1;
}

