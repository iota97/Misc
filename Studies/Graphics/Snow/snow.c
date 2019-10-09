// Snow simulator test

#define SPEED 10
#define SPAWN_RATEO 10
#define FLUIDITY 1

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

void sleep_ms(int milliseconds) // cross-platform sleep function
{
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}

static char screen[80][23];

int load_map(char* path)
{
	FILE* file = fopen(path, "r");
	
	if (file == NULL)
		return 0;

	for (int y = 0; y < 23; y++)
	{
		for (int x = 0; x < 80; x++)
			screen[x][y] = fgetc(file);
		fgetc(file);
	}
	
	return 1;
}

void display()
{
	for (int y = 0; y < 23; y++)
	{
		for (int x = 0; x < 80; x++)
			putchar(screen[x][y]);

		putchar('\n');
	}

	return;
}

void update()
{
	// Spawn
	for (int x = 0; x < 80; x++)
	{
		if (screen[x][0] == ' ' && rand() % (int)(1000/SPAWN_RATEO) == 0)
			screen[x][0] = '*';
	}

	// Process
	for (int y = 22; y >= 0; y--)
	{
		for (int x = 0; x < 80; x++)
		{

			// Below screen
			if (y == 22 && screen[x][y] == '*')
				screen[x][y] = ' ';

			// Move
			if (screen[x][y] == '*')
			{
				// Drop
				if (screen[x][y+1] == ' ')
				{
					screen[x][y] = ' ';
					screen[x][y+1] = '*';
				}

				// Slide
				else
				{
					int r = 1 - 2*(rand()%2);
				
					for(int i = 1; i <= FLUIDITY; i++)
					{
						// Side boundaries?
						if (x+r*i != -1 && x+r*i != 80)
						{	
							if (screen[x+r*i][y] == ' ')
							{
								screen[x+r*(i-1)][y] = ' ';
								screen[x+r*i][y] = '*';
							}
							else
								break;	
						}
						else
						{
							screen[x+r*(i-1)][y] = ' ';
						}
					}
				}
			}
		}
	}

	return;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Give map file as first argument.\n");
		return 1;
	}
	if (!load_map(argv[1]))
	{
		printf("Error opening file %s.\n", argv[1]);
		return 2;
	}

	for (;;)
	{
		display();
		update();
		sleep_ms((int)1000/SPEED);
	}	

	return 0;
}
