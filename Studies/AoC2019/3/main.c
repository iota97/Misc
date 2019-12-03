/* Advent of code 2019 - 3 */

#include <stdio.h>
#include <stdlib.h>

#define S 1024*16

static char map[S][S];
static char buffer[8192];
static int cur_x = S/2;
static int cur_y = S/2;
static int canary;

void update(char dir, int val, int wire)
{
	int x, y;
	switch(dir)
	{
		case('U'):
			y = 0;
			x = -1;
			break;
		case('D'):
			y = 0;
			x = 1;
			break;
		case('L'):
			y = -1;
			x = 0;
			break;
		case('R'):
			y = 1;
			x = 0;
			break;
	}
	for (int i = 0; i < val; i++)
	{
		cur_x += x;
		cur_y += y;
		if (wire == 0)
		{
			map[cur_x][cur_y] = '*';
		}
		else
		{
			if (map[cur_x][cur_y] == '*')
				map[cur_x][cur_y] = 'X';
			else
				map[cur_x][cur_y] = '*';
		}
	}
}

int path(char dir, int val, int tx, int ty)
{
	int x, y;
	int s = 0;
	switch(dir)
	{
		case('U'):
			y = 0;
			x = -1;
			break;
		case('D'):
			y = 0;
			x = 1;
			break;
		case('L'):
			y = -1;
			x = 0;
			break;
		case('R'):
			y = 1;
			x = 0;
			break;
	}
	for (int i = 0; i < val; i++)
	{
		cur_x += x;
		cur_y += y;
		s++;
		if (cur_x == tx && cur_y == ty)
		{
			canary = 1;
			return s;
		}
	}

	return s;
}

int main(int argc, char** argv)
{
	/* Check comand line */
	if (argc < 2)
	{
		puts("Please provide input file path");
		return 1;
	}

	/* Input parse init */
	FILE* input_file = fopen(argv[1], "r");

	/* Check the read to be succefull */
	if (input_file == NULL)
	{
		printf("Error reading file %s\n", path);
		return 1;
	}

	/* Parse wires */
	int val;
	char dir;

	for (int i = 0; i < S; i++)
		for (int j = 0; j < S; j++)
			map[i][j] = ' ';
	
	/* Write 1 */
	cur_x = S/2;
	cur_y = S/2;
	fgets(buffer, 8192, input_file);
	while (sscanf(buffer, "%c%d,%s", &dir, &val, buffer) == 3)
	{	
		update(dir, val, 0);
	}
	sscanf(buffer, "%c%d", &dir, &val);
	update(dir, val, 0);

	/* Write 2 */
	cur_x = S/2;
	cur_y = S/2;
	fgets(buffer, 8192, input_file);
	while (sscanf(buffer, "%c%d,%s", &dir, &val, buffer) == 3)
	{	
		update(dir, val, 1);
	}
	sscanf(buffer, "%c%d", &dir, &val);
	update(dir, val, 1);

	int dist = S*2;
	int dist2 = S*S;

	map[S/2][S/2] = 'o';
	for (int i = 0; i < S; i++)
		for (int j = 0; j < S; j++)
			if (map[i][j] == 'X')
			{
				int x = (i-S/2) > 0 ? (i-S/2) : -(i-S/2);
				int y = (j-S/2) > 0 ? (j-S/2) : -(j-S/2);
				int new = x+y;
				if (new < dist)
					dist = new;

				/* Write 1 */
				new = 0;
				rewind(input_file);
				cur_x = S/2;
				cur_y = S/2;
				fgets(buffer, 8192, input_file);
				canary = 0;
				while (sscanf(buffer, "%c%d,%s", &dir, &val, buffer) == 3)
				{	
					if (!canary)
						new += path(dir, val, i, j);
				}
				sscanf(buffer, "%c%d", &dir, &val);
				if (!canary)
						new += path(dir, val, i, j);
				
				/* Write 2 */
				cur_x = S/2;
				cur_y = S/2;
				fgets(buffer, 8192, input_file);
				canary = 0;
				while (sscanf(buffer, "%c%d,%s", &dir, &val, buffer) == 3)
				{	
					if (!canary)
						new += path(dir, val, i, j);
				}
				sscanf(buffer, "%c%d", &dir, &val);
				if (!canary)
						new += path(dir, val, i, j);
				
				if (new < dist2)
					dist2 = new;	
			}

	fclose(input_file);
	printf("%d, %d\n", dist, dist2);
	return 0;
}

