// C Multi dimensional array allocation order

#define X 2
#define Y 3
#define Z 4
#define W 5

#include <stdio.h>

int main()
{
	typedef struct 
	{
		int x;
		int y;
		int z;
		int w;
	} val;

	val arr[X][Y][Z][W];

	for (int x = 0; x < X; x++)
	{
		for (int y = 0; y < Y; y++)
		{
			for (int z = 0; z < Z; z++)
			{
				for (int w = 0; w < W; w++)
				{
					arr[x][y][z][w].x = x;
					arr[x][y][z][w].y = y;
					arr[x][y][z][w].z = z;
					arr[x][y][z][w].w = w;
				}
			}
		}
	}

	/*
	for (int w = 0; w < W; w++)
	{
		for (int z = 0; z < Z; z++)
		{
			for (int y = 0; y < Y; y++)
			{
				for (int x = 0; x < X; x++)
				{
					arr[x][y][z][w].x = x;
					arr[x][y][z][w].y = y;
					arr[x][y][z][w].z = z;
					arr[x][y][z][w].w = w;
				}
			}
		}
	}	
	*/

	for (int off = 0; off < X*Y*Z*W; off++)
	{
		printf("Offset %d -> arr[%d][%d][%d][%d]\n",
			off,
			((val*)arr+off)->x,
			((val*)arr+off)->y,
			((val*)arr+off)->z,
			((val*)arr+off)->w);

		getchar();
	}

	return 0;
}
