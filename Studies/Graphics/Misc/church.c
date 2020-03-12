// cc church.c -O2 -o church -lncurses

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#define PI 3.14159265358979f
#define NEAR_PLANE 0.2f
#define LIGHT_POS_X 1.0f
#define LIGHT_POS_Y 2.0f
#define LIGHT_POS_Z 0.0f
#define SHADOW_CHAR '!'
#define LIGHT_CHAR '#'
#define FONT_RATEO 0.5f
#define CAM_ROT 0.1f
#define CAM_MOT 0.5f

int parse_obj(char* path);
void translate(float x, float y, float z);
void update_transform(float update[4][4]);
void scale(float x, float y, float z);
void rotate_x(float x);
void rotate_y(float y);
void rotate_z(float z);
void clear_buffer(void);
void restore_mesh(void);
void render_to_buffer(void);
void clear_screen(void);
void draw_screen(void);
void show_help(void);
void create_buffer(int width, int height);
void loop_input(void);

/* Vertex stuct */
typedef struct vertex 
{
	float x;
	float y;
	float z;
} vertex_t;

/* Tris and vertex buffer */
static int tris_count = 0;
static int *tris_buffer = NULL;
static vertex_t* vertex_buffer = NULL;

/* Screen and depth buffer */
static int buffer_width = 0;
static int buffer_height = 0;
static char *screen_buffer = NULL;
static float *depth_buffer = NULL;

/* Screen rateo based on screen height, width and font rateo */
static float screen_rateo;

/* Transform matrix */
static float transform[4][4];

/* Normalize rotation between 0 and 2PI */
float normalized_angle(float x)
{
	return (x < 0) ? x + 2*PI*((int)(-x/(2*PI))+1) : x - 2*PI*(int)(x/(2*PI));
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
int parse_obj(char* path) 
{
	static unsigned int vertex_count = 0;
	char line_buffer[1024];
	FILE* mesh_file = fopen(path, "r");

	/* Check the read to be succefull */
	if (mesh_file == NULL)
	{
		printf("Error reading file %s\n", path);
		return 1;
	}	

	/* Cycle the file once to get the vertex and tris count */
	while(fgets(line_buffer, 1024, mesh_file)) 
	{
		/* Count vertex */
		if (line_buffer[0] == 'v' && line_buffer[1] == ' ') 
			vertex_count++;
		
		/* Count tris */
		else if (line_buffer[0] == 'f' && line_buffer[1] == ' ')
		{
			int test_vertex;

			/* Parse the first tris */
			sscanf(line_buffer, "%*s %*s %*s %*s %[^\n]", line_buffer);

			/* Increase tris count */
			tris_count++;
			
			/* Parse other tris if the face have more vertex */
			while (sscanf(line_buffer, "%d/", &test_vertex) == 1)
			{
				/* Increase tris count */
				tris_count++;

				/* Remove already parsed face and keep looping if other are present */
				if (sscanf(line_buffer, "%*s %[^\n]", line_buffer) != 1)
					break;
			}
		}
	}

	/* Check vertex and tris count */
	if (vertex_count == 0 || tris_count == 0)
	{
		printf("Corrupted file %s\n", path);
		return 1;
	}

	/* Free previous tris and vertex */
	free(tris_buffer);
	free(vertex_buffer);

	/* Alloc the memory */
	vertex_buffer = (vertex_t*) malloc(vertex_count * sizeof(vertex_t));
	tris_buffer = (int*) malloc(tris_count * sizeof(int) * 3);

	/* Reset the counter and rewind the file */
	vertex_count = 0;
	tris_count = 0;
	rewind(mesh_file);

	/* Cycle again, reading the data */
	while(fgets(line_buffer, 1024, mesh_file)) 
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

		/* Read face data */
		else if (line_buffer[0] == 'f' && line_buffer[1] == ' ') 
		{
			/* Parse the first tris */
			int vertex0, vertex1, vertex2;
			sscanf(line_buffer, "%*s %d/%*s %d/%*s %d/%*s %[^\n]",
				&vertex0, &vertex1, &vertex2, line_buffer);

			/* Bind the vertex */
			tris_buffer[tris_count*3+0] = vertex0-1;
			tris_buffer[tris_count*3+1] = vertex1-1;
			tris_buffer[tris_count*3+2] = vertex2-1;

			/* Increase tris count */
			tris_count++;
			
			/* Parse other tris if the face have more vertex */
			while (sscanf(line_buffer, "%d/", &vertex1) == 1)
			{
				/* Bind the vertex0, the new vertex and the last as a tris */
				tris_buffer[tris_count*3+0] = vertex0-1;
				tris_buffer[tris_count*3+1] = vertex1-1;
				tris_buffer[tris_count*3+2] = vertex2-1;

				/* Set vertex2 as our new last vertex */
				vertex2 = vertex1;			
				
				/* Increase tris count */
				tris_count++;

				/* Remove already parsed face and keep looping if other are present */
				if (sscanf(line_buffer, "%*s %[^\n]", line_buffer) != 1)
					break;
			}
		}
	}

	/* Close the file */
	fclose(mesh_file);

	return 0;
}

/* Translate the mesh */
void translate(float x, float y, float z) 
{
	/* Add translation directly to the matrix */
	transform[3][0] -= x;
	transform[3][1] += y;
	transform[3][2] += z;

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
	int row, col;
	for (row = 0; row < buffer_height; row++)
	{	
		for (col = 0; col < buffer_width; col++)
		{
			screen_buffer[col+row*buffer_width] = ' ';
			depth_buffer[col+row*buffer_width] = 0;
		}
	}

	return;
}

/* Restore position */
void restore_mesh()
{
	int row, col;
	
	/* Restore identity matrix */
	for (col = 0; col < 4; col++)
	{
		for (row = 0; row < 4; row++)
		{
			if (col == row)
				transform[col][row] = 1;
			else
				transform[col][row] = 0;
		}
	}

	return;
}

/* Render to screen buffer */
void render_to_buffer()
{
	int tris, vertex;
	int x, y;

	/* Used for near plane culling when we split a tris in 2 */
	int last_half_rendered = 0;

	/* Clear before start */
	clear_buffer();

	for (tris = 0; tris < tris_count; tris++) 
	{
		/* Barycentric coordinate determinant */
		float determinant;

		/* Get the bounding coordinate of the tris */
		int min_x = buffer_width-1;
		int min_y = buffer_height-1;
		int max_x = 0;
		int max_y = 0;
		int behind_near = 0;

		/* Transformed vertex */
		vertex_t vertex_arr[3];

		/* Face normal and lighting */
		vertex_t normal, edge0, edge1;
		float normal_mag, light;

		for (vertex = 0; vertex < 3; vertex++)
		{
			/* Multiply with transform matrix */
			vertex_arr[vertex].x = transform[0][0]*vertex_buffer[tris_buffer[tris*3+vertex]].x+
					transform[1][0]*vertex_buffer[tris_buffer[tris*3+vertex]].y+
					transform[2][0]*vertex_buffer[tris_buffer[tris*3+vertex]].z+
					transform[3][0];

			vertex_arr[vertex].y = transform[0][1]*vertex_buffer[tris_buffer[tris*3+vertex]].x+
					transform[1][1]*vertex_buffer[tris_buffer[tris*3+vertex]].y+
					transform[2][1]*vertex_buffer[tris_buffer[tris*3+vertex]].z+
					transform[3][1];

			vertex_arr[vertex].z = transform[0][2]*vertex_buffer[tris_buffer[tris*3+vertex]].x+
					transform[1][2]*vertex_buffer[tris_buffer[tris*3+vertex]].y+
					transform[2][2]*vertex_buffer[tris_buffer[tris*3+vertex]].z+
					transform[3][2];

			/* Count vertex behind near plane */
			if (vertex_arr[vertex].z < NEAR_PLANE)
				behind_near++;
		}

		/* Near plane clipping nad culling */
		if (behind_near > 0)
		{
			/* If all vertex are behind */
			if (behind_near == 3)
			{
				/* Skip the tris */
				continue;
			}

			/* If we have 1 or 2 vertex behind */
			else
			{
				/* Find the 2 point of intersection with the near plane */
				vertex_t intersect[2];
				int id[3];

				/* Set id[0] as id of the one on the different side */
				if ((vertex_arr[0].z >= NEAR_PLANE && behind_near == 2) ||
					(vertex_arr[0].z < NEAR_PLANE && behind_near == 1))
				{
					id[0] = 0;
					id[1] = 1;
					id[2] = 2;
				}
				else if ((vertex_arr[1].z >= NEAR_PLANE && behind_near == 2) ||
					(vertex_arr[1].z < NEAR_PLANE && behind_near == 1))
				{
					id[0] = 1;
					id[1] = 0;
					id[2] = 2;
				}
				else
				{
					id[0] = 2;
					id[1] = 0;
					id[2] = 1;
				}

				/* Calculate the intersection point, check alignment first */
				intersect[0].x = (vertex_arr[id[0]].x == vertex_arr[id[1]].x) ? vertex_arr[id[0]].x :
						vertex_arr[id[0]].x + (NEAR_PLANE-vertex_arr[id[0]].z)*
						(vertex_arr[id[0]].x-vertex_arr[id[1]].x)/(vertex_arr[id[0]].z-vertex_arr[id[1]].z);

				intersect[0].y = (vertex_arr[id[0]].y == vertex_arr[id[1]].y) ? vertex_arr[id[0]].y :
						vertex_arr[id[0]].y + (NEAR_PLANE-vertex_arr[id[0]].z)*
						(vertex_arr[id[0]].y-vertex_arr[id[1]].y)/(vertex_arr[id[0]].z-vertex_arr[id[1]].z);
				
				intersect[1].x = (vertex_arr[id[0]].x == vertex_arr[id[2]].x) ? vertex_arr[id[0]].x :
						vertex_arr[id[0]].x + (NEAR_PLANE-vertex_arr[id[0]].z)*
						(vertex_arr[id[0]].x-vertex_arr[id[2]].x)/(vertex_arr[id[0]].z-vertex_arr[id[2]].z);

				intersect[1].y = (vertex_arr[id[0]].y == vertex_arr[id[2]].y) ? vertex_arr[id[0]].y :
						vertex_arr[id[0]].y + (NEAR_PLANE-vertex_arr[id[0]].z)*
						(vertex_arr[id[0]].y-vertex_arr[id[2]].y)/(vertex_arr[id[0]].z-vertex_arr[id[2]].z);
				
				/* Create new tris */
				if (behind_near == 2)
				{
					/* The vertex in front is kept, the other 2 become the intersect */
					vertex_arr[0].x = vertex_arr[id[0]].x;
					vertex_arr[0].y = vertex_arr[id[0]].y;
					vertex_arr[0].z = vertex_arr[id[0]].z;

					vertex_arr[1].x = intersect[0].x;
					vertex_arr[1].y = intersect[0].y;
					vertex_arr[1].z = NEAR_PLANE;

					vertex_arr[2].x = intersect[1].x;
					vertex_arr[2].y = intersect[1].y;
					vertex_arr[2].z = NEAR_PLANE;
				}
				else
				{
					/* If we have 2 vertex in front we need to create 2 tris */
					last_half_rendered = !last_half_rendered;

					/* Check with tris we rendered last time */
					if (last_half_rendered)
					{
						/* Create the first tris */
						vertex_t tmp;

						tmp.x = vertex_arr[id[1]].x;
						tmp.y = vertex_arr[id[1]].y;
						tmp.z = vertex_arr[id[1]].z;

						vertex_arr[2].x = vertex_arr[id[2]].x;
						vertex_arr[2].y = vertex_arr[id[2]].y;
						vertex_arr[2].z = vertex_arr[id[2]].z;
	
						vertex_arr[1].x = tmp.x;
						vertex_arr[1].y = tmp.y;
						vertex_arr[1].z = tmp.z;

						vertex_arr[0].x = intersect[0].x;
						vertex_arr[0].y = intersect[0].y;
						vertex_arr[0].z = NEAR_PLANE;

						/* Decrease tris, so we get the same tris next cycle */
						tris--;
					}
					else
					{	
						/* Create the second tris */
						vertex_arr[2].x = vertex_arr[id[2]].x;
						vertex_arr[2].y = vertex_arr[id[2]].y;
						vertex_arr[2].z = vertex_arr[id[2]].z;
				
						vertex_arr[0].x = intersect[1].x;
						vertex_arr[0].y = intersect[1].y;
						vertex_arr[0].z = NEAR_PLANE;

						vertex_arr[1].x = intersect[0].x;
						vertex_arr[1].y = intersect[0].y;
						vertex_arr[1].z = NEAR_PLANE;
					}
				}
			}
		}

		/* Compute face normal as cross product */
		edge0.x = vertex_arr[0].x-vertex_arr[2].x;
		edge0.y = vertex_arr[0].y-vertex_arr[2].y;
		edge0.z = vertex_arr[0].z-vertex_arr[2].z;
		edge1.x = vertex_arr[1].x-vertex_arr[2].x;
		edge1.y = vertex_arr[1].y-vertex_arr[2].y;
		edge1.z = vertex_arr[1].z-vertex_arr[2].z;

		normal.x = edge0.y*edge1.z - edge0.z*edge1.y;
		normal.y = edge0.z*edge1.x - edge0.x*edge1.z;
		normal.z = edge0.x*edge1.y - edge0.y*edge1.x;
	
		/* Compute light factor */
		light = normal.x*-LIGHT_POS_X+normal.y*LIGHT_POS_Y+normal.z*LIGHT_POS_Z;
		
		/* Flip normal if its a backward */
		if (normal.z > 0) 
			light *= -1;
		
		/* Raster vertex to screen */
		for (vertex = 0; vertex < 3; vertex++)
		{
			vertex_arr[vertex].x = (vertex_arr[vertex].x / -vertex_arr[vertex].z * buffer_width) + buffer_width/2;
			vertex_arr[vertex].y = (vertex_arr[vertex].y / -vertex_arr[vertex].z * buffer_height)*screen_rateo + buffer_height/2;

			/* Get boundaries */
			if (vertex_arr[vertex].x < min_x)
				min_x = (int) vertex_arr[vertex].x;
			if (vertex_arr[vertex].x > max_x)
				max_x = (int) vertex_arr[vertex].x;
			if (vertex_arr[vertex].y < min_y)
				min_y = (int) vertex_arr[vertex].y;
			if (vertex_arr[vertex].y > max_y)
				max_y = (int) vertex_arr[vertex].y;
		}

		/* Check boundaries */
		max_x = (max_x > buffer_width-1) ? buffer_width-1 : max_x;
		max_y = (max_y > buffer_height-1) ? buffer_height-1 : max_y;
		min_x = (min_x < 0) ? 0 : min_x;
		min_y = (min_y < 0) ? 0 : min_y;
		
		/* Calculate the determinant for barycentric coordinate */
		determinant = 1/((vertex_arr[1].y-vertex_arr[2].y)*(vertex_arr[0].x-vertex_arr[2].x) +
					(vertex_arr[2].x-vertex_arr[1].x)*(vertex_arr[0].y-vertex_arr[2].y));

		/* Test only the pixel in this area */
		for (y = min_y; y <= max_y; y++) 
		{
			for (x = min_x; x <= max_x; x++) 
			{
				/* Calculate barycentric coordinate */
				float lambda0 = ((vertex_arr[1].y-vertex_arr[2].y)*(x-vertex_arr[2].x) +
					(vertex_arr[2].x-vertex_arr[1].x)*(y-vertex_arr[2].y))*determinant;
				float lambda1 = ((vertex_arr[2].y-vertex_arr[0].y)*(x-vertex_arr[2].x) +
					(vertex_arr[0].x-vertex_arr[2].x)*(y-vertex_arr[2].y))*determinant;
				float lambda2 = 1.0f - lambda0 - lambda1;
				
				/* If is inside the triangle, render it */
				if (lambda0 >= 0 && lambda1 >= 0 && lambda2 >= 0) 
				{
					/* Interpolate Z value */
					float pixel_depth;
			
					/* Perspective correct interpolation */
					pixel_depth = lambda0 / vertex_arr[0].z +
							lambda1 / vertex_arr[1].z +
							lambda2 / vertex_arr[2].z;


					/* Test depth buffer */
					if (depth_buffer[x+y*buffer_width] < pixel_depth)
					{
						/* Update both buffer */	
						depth_buffer[x+y*buffer_width] = pixel_depth;
						screen_buffer[x+y*buffer_width] = light < 0 ? SHADOW_CHAR : LIGHT_CHAR;
					}
				}
			}			
		}
	}

	return;
}

/* Draw in the console */
void draw_screen()
{
	int col, row;

	/* Render to buffer */
	render_to_buffer();
	
	/* Clear the console */
	clear();

	/* Print its */
	for (row = 0; row < buffer_height; row++)
	{	
		for (col = 0; col < buffer_width; col++)
		{
			/* Print the char in the buffer */
			addch(screen_buffer[col+row*buffer_width]);
		}
	}

	return;
}

/* Create depth and screen buffer */
void create_buffer(int width, int height)
{
	/* Check width and height to be more than zero */
	if (width <= 0 || height <= 0)
		return;

	/* Free old buffer */
	free(screen_buffer);
	free(depth_buffer);

	/* Allocate new one */
	screen_buffer = (char*) malloc(sizeof(char) * width * height);
	depth_buffer = (float*) malloc(sizeof(float) * width * height);

	/* Set global buffer size */
	buffer_width = width;
	buffer_height = height;

	/* Update screen rateo */
	screen_rateo = (float)buffer_width/buffer_height*FONT_RATEO;

	return;
}

/* Ncurses input loop */
void loop_input()
{
	/* Input variables */
	int command;
	int quit = 0;

	while (!quit)
	{
		/* Check screen to be the same size as before */
		if (buffer_width != getmaxx(stdscr) || buffer_height != getmaxy(stdscr))
		{
			create_buffer(getmaxx(stdscr), getmaxy(stdscr));
		}

		/* Render */
		draw_screen();

		/* Get input */
		command = getch();
	
		switch (command)
		{
			/* Quit */
			case ('\n'):
				quit = 1;
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

/* Main */
int main(int argc, char *argv[]) 
{
	/* Parse the model */
	parse_obj("church.obj");

	/* Ncurses init */
	initscr();
	cbreak();
	noecho();
	curs_set(0);

	/* Restore the mesh */
	restore_mesh();

	/* Allocate rendering buffer */
	create_buffer(getmaxx(stdscr), getmaxy(stdscr));

	/* Start the input loop */
	loop_input();

	/* Free memory */
	free(tris_buffer);
	free(vertex_buffer);
	free(screen_buffer);
	free(depth_buffer);
	
	/* Kill the windows */
	endwin();

	/* Exit */
	return 0;
}
