// Snow simulator test

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
		if (screen[x][0] == ' ' && rand() % 20 == 0)
			screen[x][0] = '*';
	}

	// Drop
	for (int y = 24; y >= 0; y--)
	{
		for (int x = 0; x < 80; x++)
		{

			if (y == 24 && screen[x][y] == '*')
				screen[x][24] = ' ';

			if (screen[x][y] == '*')
			{
				if (screen[x][y+1] == ' ')
				{
					screen[x][y] = ' ';
					screen[x][y+1] = '*';
				}

				else
				{
					int r = 1 - 2*(rand()%2);

					if (x+r != -1 && x+r != 80)
					{	
						if (screen[x+r][y] == ' ')
						{
							screen[x][y] = ' ';
							screen[x+r][y] = '*';
						}
					}
					else
						screen[x][y] = ' ';
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
		sleep_ms(100);
	}	

	return 0;
}
