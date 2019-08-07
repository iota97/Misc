/* 6DoF 3D model church 

Compile with:

cc church.c -O2 -o church -lncurses

Controls:

W - forward
S - backward
A - left
D - right

SPACE - up
C - down

I - camera up
K - camera down
J - camera left
L - camera right

Q - camera roll left
E - camera roll right

ENTER - exit

*/

/* Headers */
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

/* Speed config */
#define CAM_ROT 0.1f
#define CAM_MOT 0.5f

/* Math const */
#define PI 3.14159265359f

/* Rendering const */
#define NEAR_PLANE 1
#define FAR_PLANE 8192
#define FOG_COLOR 1

/* Font width/height rateo */
#define FONT_RATEO 0.5f

/* Vertex stuct */
typedef struct vertex 
{
	float x;
	float y;
	float z;
	int color;
} vertex_t;

/* Tris counter */
static unsigned int tris_count = 0;

/* Tris Buffer */
static vertex_t *tris_buffer = NULL;

/* Screen and depth buffer */
static int buffer_width = 0;
static int buffer_height = 0;
static int *screen = NULL;
static float *depth = NULL;

/* Screen rateo based on screen height, width and font rateo */
static float screen_rateo;

/* Transform matrix */
static float transform[4][4];

/* Prototype */
float absolute(float x);
float normalized_angle(float x);
float sine(float x);
float cosine(float x);
void load_world(void);
void update_transform(float update[4][4]);
void move_camera(float x, float y, float z);
void rotate_x(float x);
void rotate_y(float y);
void rotate_z(float z);
void clear_buffer(void);
void restore_position(void);
void render_to_buffer(void);
void draw(void);
void create_buffer(int width, int height);
void exit_game(void);
void loop_game(void);
void init_game(void);

/* Calculate the absolute value */
float absolute(float x)
{
	return x > 0 ? x : -x;
}

/* Normalize rotation between 0 and 2PI */
float normalized_angle(float x)
{
	if (x < 0)
		x += 2*PI*((int)(-x/(2*PI))+1);
	else 
		x -= 2*PI*(int)(x/(2*PI));

	return x;
}	

/* Sine */
float sine(float x)
{
	float sign;

	/* Normalize the rotation */
	x = normalized_angle(x);
		
	/* Check sign */
	if (x < PI)
		sign = 1;
	else
		sign = -1;

	/* Check symmetry */
	if (x < PI/2 || (x >= PI && x < 3*PI/2))
		x -= (PI/2)*(int)(x/(PI/2));
	else
		x = PI/2 - x + (PI/2)*(int)(x/(PI/2));

	/* Check it to be below 45 degree */
	if (x < PI/4)
	{
		/* Polinomial approximation */
		float x2 = x*x;
		float x3 = x2*x;
		float sine = x - x3/6 + x2*x3/120;

		return sine*sign;
	}
	else
	{
		float x2, x4, x6, cosine;

		/* Transform to cosine */
		x = PI/2 - x;

		/* Polinomial approximation */
		x2 = x*x;
		x4 = x2*x2;
		x6 = x2*x4;
		cosine = 1 - x2/2 + x4/24 - x6/720;

		return cosine*sign;
	}
}

/* Cosine */
float cosine(float x)
{
	float sign;

	/* Normalize the rotation */
	x = normalized_angle(x);
		
	/* Check sign */
	if (x < PI/2 || x >= 3*PI/2)
		sign = 1;
	else
		sign = -1;

	/* Check symmetry */
	if (x < PI/2 || (x >= PI && x < 3*PI/2))
		x -= (PI/2)*(int)(x/(PI/2));
	else
		x = PI/2 - x + (PI/2)*(int)(x/(PI/2));

	/* Check it to be below 45 degree */
	if (x < PI/4)
	{
		/* Polinomial approximation */
		float x2 = x*x;
		float x4 = x2*x2;
		float x6 = x2*x4;
		float cosine = 1 - x2/2 + x4/24 - x6/720;

		return cosine*sign;
	}
	else
	{
		float x2, x3, sine;
	
		/* Transform to sine */
		x = PI/2 - x;

		/* Polinomial approximation */
		x2 = x*x;
		x3 = x2*x;
		sine = x - x3/6 + x2*x3/120;

		return sine*sign;
	}
}

/* Read the mesh file */
void load_world() 
{
	int vertex_count = 0;
	int color = 0;
	char line_buffer[1024];
	FILE* world_file = fopen("church.obj", "r");
	vertex_t* vertex_buffer = NULL;

	/* Cycle the file once to get the vertex and tris count */
	while(fgets(line_buffer, 1024, world_file)) 
	{
		/* Count vertex */
		if (line_buffer[0] == 'v' && line_buffer[1] == ' ') 
			vertex_count++;
		
		/* Count tris */
		else if (line_buffer[0] == 'f' && line_buffer[1] == ' ')
		{
			/* Increase tris count */
			tris_count++;
		}
	}

	/* Check to have tris and vertex */
	if (tris_count == 0 || vertex_count == 0)
		return;

	/* Free previous tris  */
	free(tris_buffer);

	/* Alloc the memory */
	vertex_buffer = (vertex_t*) malloc((unsigned int)vertex_count * sizeof(vertex_t));
	tris_buffer = (vertex_t*) malloc((unsigned int)tris_count * sizeof(vertex_t) * 3);

	/* Reset the counter and rewind the file */
	vertex_count = 0;
	tris_count = 0;
	rewind(world_file);

	/* Cycle again, reading the data */
	while(fgets(line_buffer, 1024, world_file)) 
	{
		/* Read vertex data */
		if (line_buffer[0] == 'v' && line_buffer[1] == ' ') 
		{
			sscanf(line_buffer, "%*s %f %f %f", 
				&vertex_buffer[vertex_count].x, 
				&vertex_buffer[vertex_count].y, 
				&vertex_buffer[vertex_count].z);

			vertex_count++;
		}

		/* Read material */
		else if (line_buffer[0] == 'u')
		{
			sscanf(line_buffer, "%*s %u", &color);
		}

		/* Read face data */
		else if (line_buffer[0] == 'f' && line_buffer[1] == ' ') 
		{
			/* Parse the first tris */
			int vertex0, vertex1, vertex2;
			sscanf(line_buffer, "%*s %u/%*s %u/%*s %u/%*s %[^\n]",
				&vertex0, &vertex1, &vertex2, line_buffer);

			/* Bind the vertex */
			tris_buffer[tris_count*3+0] = vertex_buffer[vertex0-1];
			tris_buffer[tris_count*3+1] = vertex_buffer[vertex1-1];
			tris_buffer[tris_count*3+2] = vertex_buffer[vertex2-1];

			/* Bind the color to the first vertex */
			tris_buffer[tris_count*3].color = color;

			/* Increase tris count */
			tris_count++;
		}
	}

	/* Free the vertex buffer */
	free(vertex_buffer);

	/* Close the file */
	fclose(world_file);

	return;
}

/* Update transform matrix */
void update_transform(float update[4][4])
{
	int i, j, k;

	/* Copy old transform */
	float copy[4][4];
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			copy[i][j] = transform[i][j];
		}
	}

	/* Multiply the update * transform using the copy */
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			transform[i][j] = 0;

			for (k = 0; k < 4; k++)
			{
				transform[i][j] += update[k][j]*copy[i][k];
			}
		}
	}

	return;
}

/* Move */
void move_camera(float x, float y, float z)
{
	int i, j;
	float update[4][4];

	/* Create empty matrix */
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			update[i][j] = 0;
		}
	}

	/* Add motion */
	update[3][0] = x;
	update[3][1] = y;
	update[3][2] = z;
	update[0][0] = 1.f;
	update[1][1] = 1.f;
	update[2][2] = 1.f;
	update[3][3] = 1.f;

	/* Update global matrix */
	update_transform(update);

	return;
}

/* Rotate on X axis */
void rotate_x(float x)
{
	int i, j;
	float update[4][4];

	/* Create empty matrix */
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			update[i][j] = 0;
		}
	}

	/* Add rotation */
	update[1][1] = cosine(x);
	update[1][2] = sine(x);
	update[2][2] = update[1][1];
	update[2][1] = -update[1][2];
	update[0][0] = 1.f;
	update[3][3] = 1.f;

	/* Update global matrix */
	update_transform(update);

	return;
}

/* Rotate on Y axis */
void rotate_y(float y)
{
	int i, j;
	float update[4][4];

	/* Create empty matrix */
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			update[i][j] = 0;
		}
	}

	/* Add rotation */
	update[0][0] = cosine(y);
	update[2][0] = -sine(y);
	update[0][2] = -update[2][0];
	update[2][2] = update[0][0];
	update[1][1] = 1.f;
	update[3][3] = 1.f;

	/* Update global matrix */
	update_transform(update);

	return;
}

/* Rotate on Z axis */
void rotate_z(float z)
{
	int i, j;
	float update[4][4];

	/* Create empty matrix */
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			update[i][j] = 0;
		}
	}

	/* Add rotation */
	update[0][0] = cosine(-z);
	update[0][1] = sine(-z);
	update[1][0] = -update[0][1];
	update[1][1] = update[0][0];
	update[2][2] = 1.f;
	update[3][3] = 1.f;

	/* Update global matrix */
	update_transform(update);

	return;
}

/* Clear the screen and depth buffer */
void clear_buffer()
{
	int i, j;
	for (i = 0; i < buffer_width; i++)
	{
		for (j = 0; j < buffer_height; j++)
		{
			screen[i+j*buffer_width] = FOG_COLOR;
			depth[i+j*buffer_width] = FAR_PLANE;
		}
	}

	return;
}

/* Restore position */
void restore_position()
{
	unsigned int i, j;
	
	/* Restore identity matrix */
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (i == j)
				transform[i][j] = 1;
			else
				transform[i][j] = 0;
		}
	}

	return;
}

/* Render to screen buffer */
void render_to_buffer()
{
	unsigned int i, j;
	int x, y;

	/* Clear before start */
	clear_buffer();

	for (i = 0; i < tris_count; i++) 
	{
		/* Barycentric coordinate determinant */
		float determinant;

		/* Get the bounding coordinate of the tris */
		int min_x = buffer_width-1;
		int min_y = buffer_height-1;
		int max_x = 0;
		int max_y = 0;

		/* Raster the vertex to screen */
		float x_array[3], y_array[3];

		/* Transformed vertex */
		vertex_t vertex[3];

		for (j = 0; j < 3; j++)
		{
			/* Multiply with transform matrix */
			vertex[j].x = transform[0][0]*tris_buffer[i*3+j].x+
					transform[1][0]*tris_buffer[i*3+j].y+
					transform[2][0]*tris_buffer[i*3+j].z+
					transform[3][0];

			vertex[j].y = transform[0][1]*tris_buffer[i*3+j].x+
					transform[1][1]*tris_buffer[i*3+j].y+
					transform[2][1]*tris_buffer[i*3+j].z+
					transform[3][1];

			vertex[j].z = transform[0][2]*tris_buffer[i*3+j].x+
					transform[1][2]*tris_buffer[i*3+j].y+
					transform[2][2]*tris_buffer[i*3+j].z+
					transform[3][2];

			/* Raster vertex to screen */
			x_array[j] = (vertex[j].x / -absolute(vertex[j].z) * buffer_width) + buffer_width/2;
			y_array[j] = (vertex[j].y / -absolute(vertex[j].z) * buffer_height)*screen_rateo + buffer_height/2;
		}

		/* Get boundaries */
		for (j = 0; j < 3; j++)
		{
			if (x_array[j] < min_x)
				min_x = (int) x_array[j];
			if (x_array[j] > max_x)
				max_x = (int) x_array[j];
			if (y_array[j] < min_y)
				min_y = (int) y_array[j];
			if (y_array[j] > max_y)
				max_y = (int) y_array[j];
		}
		
		/* Check boundaries */
		max_x = (max_x > buffer_width-1) ? buffer_width-1 : max_x;
		max_y = (max_y > buffer_height-1) ? buffer_height-1 : max_y;
		min_x = (min_x < 0) ? 0 : min_x;
		min_y = (min_y < 0) ? 0 : min_y;
		
		/* Calculate the determinant for barycentric coordinate */
		determinant = 1/((y_array[1]-y_array[2])*(x_array[0]-x_array[2]) +
					(x_array[2]-x_array[1])*(y_array[0]-y_array[2]));

		/* Test only the pixel in this area */
		for (y = min_y; y <= max_y; y++) 
		{
			for (x = min_x; x <= max_x; x++) 
			{
				/* Calculate barycentric coordinate */
				float lambda0 = ((y_array[1]-y_array[2])*(x-x_array[2]) +
					(x_array[2]-x_array[1])*(y-y_array[2]))*determinant;
				float lambda1 = ((y_array[2]-y_array[0])*(x-x_array[2]) +
					(x_array[0]-x_array[2])*(y-y_array[2]))*determinant;
				float lambda2 = 1 - lambda0 - lambda1;
				
				/* If is inside the triangle, render it */
				if (lambda0 >= 0 && lambda1 >= 0 && lambda2 >= 0) 
				{
					/* Interpolate Z value */
					float pixel_depth = vertex[0].z * lambda0 +
								vertex[1].z * lambda1 +
								vertex[2].z * lambda2;

					/* Test depth buffer and near plane */
					if (depth[x+y*buffer_width] > pixel_depth && pixel_depth > NEAR_PLANE)
					{
						/* Update both buffer */
						screen[x+y*buffer_width] = tris_buffer[i*3].color;
						depth[x+y*buffer_width] = pixel_depth;
					}
				}
			}			
		}
	}

	return;
}

/* Draw in the console */
void draw()
{
	int i, j;

	/* Render to buffer */
	render_to_buffer();
	
	/* Clear the console */
	clear();

	/* Print it */
	for (j = 0; j < buffer_height; j++) 
	{
		for (i = 0; i < buffer_width; i++)
		{
			/* Set color attribute and print a blank space */
			attron(COLOR_PAIR(screen[i+j*buffer_width]));
			addch(' ');
		}
	}

	return;
}

/* Create depth and screen buffer */
void create_buffer(int width, int height)
{
	/* Free old buffer */
	free(screen);
	free(depth);

	/* Allocate new one */
	screen = (int*) malloc(sizeof(int) * (unsigned int)width * (unsigned int)height);
	depth = (float*) malloc(sizeof(float) * (unsigned int)width * (unsigned int)height);

	/* Set global buffer size */
	buffer_width = (int) width;
	buffer_height = (int) height;

	/* Update screen rateo */
	screen_rateo = (float)buffer_width/buffer_height*FONT_RATEO;

	return;
}

/* Clean and exit */
void exit_game()
{
	/* Free memory */
	free(tris_buffer);
	free(screen);
	free(depth);
	
	/* Kill the windows */
	endwin();

	return;
}

/* Main loop */
void loop_game()
{
	int quit = 0;

	while(!quit)
	{
		/* Check window resize */
		if (getmaxx(stdscr) != buffer_width || getmaxy(stdscr) != buffer_height)
			create_buffer(getmaxx(stdscr), getmaxy(stdscr));
		
		/* Render */
		draw();

		/* Process input */
		switch(getch())
		{
			/* Quit */
			case ('\n'):
				quit = 1;
				exit_game();
				break;

			/* Camera rotation */
			case('l'):
				rotate_y(-CAM_ROT);
				break;

			case('j'):
				rotate_y(CAM_ROT);
				break;

			case('i'):
				rotate_x(CAM_ROT);
				break;

			case('k'):
				rotate_x(-CAM_ROT);
				break;

			case('q'):
				rotate_z(-CAM_ROT);
				break;

			case('e'):
				rotate_z(CAM_ROT);
				break;

				
			/* Camera movement */
			case('w'):
				move_camera(0, 0, -CAM_MOT);
				break;

			case('s'):
				move_camera(0, 0, CAM_MOT);
				break;

			case('a'):
				move_camera(-CAM_MOT, 0, 0);
				break;

			case('d'):
				move_camera(CAM_MOT, 0, 0);
				break;

			case('c'):
				move_camera(0, CAM_MOT, 0);
				break;

			case(' '):
				move_camera(0, -CAM_MOT, 0);
				break;
		}

	}

	return;
}

/* Init game */
void init_game()
{
	short i;
	
	/* Ncurses init */
	initscr();
	cbreak();
	noecho();
	curs_set(0);

	/* Bind color pair */
	start_color();
	for (i = 0; i < 8; i++)
	{
		/* Color from 0 to  7*/
		init_pair(i+1, COLOR_WHITE, i);
	}

	/* Rendering init */
	create_buffer(getmaxx(stdscr), getmaxy(stdscr));

	/* World init */
	load_world();
	restore_position();

	/* Start the game loop */
	loop_game();

	return;
}

/* Main */
int main()
{
	init_game();
	return 0;
}
