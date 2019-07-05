// CLI rasterization

// use tris only .obj model with far away vertex (float precision)

#include <stdio.h>
#include <math.h>

// Vertex stuct
typedef struct vertex 
{
	float x;
	float y;
	float z;
} vertex_t;


// A buffer with all tris
static vertex_t tris_buffer[8192][3];
static vertex_t mesh_tris[8192][3];
static int tris_count = 0;

// Screen and depth buffer
static char screen[80][40];
static float depth[80][40];

// Material array of char
static char material_array[] = {'a', 'b', 'c', 'd', 
				'e', 'f', 'g', 'h',
				'i', 'j', 'k', 'l',
				'm', 'n', 'o', 'p'};

// Restore position
void restore()
{
	for (int i = 0; i < tris_count; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			tris_buffer[i][j].x = mesh_tris[i][j].x; 
			tris_buffer[i][j].y = mesh_tris[i][j].y;
			tris_buffer[i][j].z = mesh_tris[i][j].z;
		}
	}

	return;
}

// Read the mesh file
int parse_obj(char* path) 
{
	char line_buffer[1024];
	FILE* mesh_file = fopen(path, "r");

	// Check the read to be succefull
	if (mesh_file == NULL)
	{
		printf("Error reading file %s\n", path);
		return 1;
	}	

	int vertex_index = 0;
	vertex_t vertex_buffer[8192];

	while(fgets(line_buffer, 1024, (FILE*) mesh_file)) 
	{
		// Read vertex data
		if (line_buffer[0] == 'v' && line_buffer[1] == ' ') 
		{
			sscanf(line_buffer, "%*s %f %f %f", 
				&vertex_buffer[vertex_index].x, 
				&vertex_buffer[vertex_index].y, 
				&vertex_buffer[vertex_index].z);

			vertex_index++;
		}

		// Read face data
		if (line_buffer[0] == 'f' && line_buffer[1] == ' ') 
		{
			int a, b, c;
			sscanf(line_buffer, "%*s %u/%*s %u/%*s %u",
				 &a, &b, &c);

			// Bind the right vertex
			mesh_tris[tris_count][0] = vertex_buffer[a-1];
			mesh_tris[tris_count][1] = vertex_buffer[b-1];
			mesh_tris[tris_count][2] = vertex_buffer[c-1];
			
			// Increase the face count
			tris_count++;
		}

	}

	// Close the file
	fclose(mesh_file);
	return 0;
}

// Translate the mesh
void translate(float x, float y, float z) 
{
	for (int i = 0; i < tris_count; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			tris_buffer[i][j].x += x; 
			tris_buffer[i][j].y += y;
			tris_buffer[i][j].z += z;
		}
	}

	return;
}

// Scale the mesh
void scale(float factor)
{
	for (int i = 0; i < tris_count; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			tris_buffer[i][j].x *= factor; 
			tris_buffer[i][j].y *= factor;
			tris_buffer[i][j].z *= factor;
		}

	}

	return;
}

// Rotate the mesh on X axis
void rotate_x(float angle)
{
	for (int i = 0; i < tris_count; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			tris_buffer[i][j].y = cos(angle) * tris_buffer[i][j].y -
						sin(angle) * tris_buffer[i][j].z;
			tris_buffer[i][j].z = sin(angle) * tris_buffer[i][j].y +
						cos(angle) * tris_buffer[i][j].z; 
		}
	}

	return;
}

// Rotate the mesh on Y axis
void rotate_y(float angle)
{
	for (int i = 0; i < tris_count; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			tris_buffer[i][j].x = cos(angle) * tris_buffer[i][j].x -
						sin(angle) * tris_buffer[i][j].z;
			tris_buffer[i][j].z = sin(angle) * tris_buffer[i][j].x +
						cos(angle) * tris_buffer[i][j].z; 
		}
	}

	return;
}

// Rotate the mesh on Z axis
void rotate_z(float angle)
{
	for (int i = 0; i < tris_count; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			tris_buffer[i][j].x = cos(angle) * tris_buffer[i][j].x -
						sin(angle) * tris_buffer[i][j].y;
			tris_buffer[i][j].y = sin(angle) * tris_buffer[i][j].x +
						cos(angle) * tris_buffer[i][j].y; 
		}
	}

	return;
}

// Clear the screen and depth buffer
void clear()
{
	for (int i = 0; i < 80; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			screen[i][j] = ' ';
			depth[i][j] = 8192;
		}
	}

	return;
}

// Render to screen buffer
void render()
{
	// Clear before start
	clear();
	int material_index = 0;

	for (int i = 0; i < tris_count; i++) 
	{
		
		// Raster the vertex to screen
		double x_array[] = {
			(tris_buffer[i][0].x / tris_buffer[i][0].z * 80) + 80/2,
			(tris_buffer[i][1].x / tris_buffer[i][1].z * 80) + 80/2,
			(tris_buffer[i][2].x / tris_buffer[i][2].z * 80) + 80/2
		};
		double y_array[] = {
			(tris_buffer[i][0].y / tris_buffer[i][0].z * 40) + 40/2,
			(tris_buffer[i][1].y / tris_buffer[i][1].z * 40) + 40/2,
			(tris_buffer[i][2].y / tris_buffer[i][2].z * 40) + 40/2
		};

		// Get the bounding coordinate of the tris
		int min_x = 80;
		int min_y = 40;
		int max_x = 0;
		int max_y = 0;

		for (int j = 0; j < 3; j++)
		{
			if (x_array[j] < min_x)
				min_x = x_array[j];
			if (x_array[j] > max_x)
				max_x = x_array[j];
			if (y_array[j] < min_y)
				min_y = y_array[j];
			if (y_array[j] > max_y)
				max_y = y_array[j];
		}
					
		// Test only the pixel in this area
		for (int y = min_y; y < max_y; y++) 
		{
			for (int x = min_x; x < max_x; x++) 
			{
				int out = 0;
				
				for (int j = 0; j < 3; j++)
				{
					// Get the next vertex index
					int j_n = (j == 2) ? 0 : j + 1;	
					int j_nn = (j_n == 2) ? 0 : j_n + 1;

					// Handle vertical line
					if (x_array[j] == x_array[j_n])
					{	
						out += (x_array[j] < x_array[j_nn]) ? (x < x_array[j]) : (x > x_array[j]);
					}
					else 
					{ 
						// Handle horizzontal one
						if (y_array[j] == y_array[j_n])
						{
							out += (y_array[j] < y_array[j_nn]) ? (y < y_array[j]) : (y > y_array[j]);
						}						
						else
						{ 
							// Check clock wise or not
							if ((x_array[j_nn] - x_array[j])*(y_array[j_n] - y_array[j]) < 
								(y_array[j_nn] - y_array[j])*(x_array[j_n] - x_array[j]))
								
								// Then
								out += (x - x_array[j])*(y_array[j_n] - y_array[j]) > 
									(y - y_array[j])*(x_array[j_n] - x_array[j]);
							else 
								// Else
								out += (x - x_array[j])*(y_array[j_n] - y_array[j]) < 
									(y - y_array[j])*(x_array[j_n] - x_array[j]);
						}					
					}
				}

				// If is inside every line, render it
				if (!out) 
				{
					// Interpolate the depth
					float d=0;
					for (int j = 0; j < 3; j++)
					{
						d += tris_buffer[i][j].z/sqrt((x-x_array[j])*(x-x_array[j]) + (y-y_array[j])*(y-y_array[j]));
					}
					
					// Test it
					if (depth[x][y] > d)
					{
						// Update both buffer
						screen[x][y] = material_array[material_index];
						depth[x][y] = d;
					}
				}

			}
			
		}

		// Render next face with another material
		material_index++;

		// Return to 0 if last element is reached
		if (material_index == sizeof(material_array)/sizeof(char))
			material_index = 0;

	}

	return;
}

// Clear the console
void clear_screen()
{	
	// Print 256 new line
	for (int i = 0; i < 256; i++)
		printf("\n");

	return;
}

// Draw in the console
void draw()
{
	// Render to buffer
	render();
	
	// Clear the console
	clear_screen();

	// Print it, cropping to 80x24
	for (int j = 12; j < 36; j++) 
	{
		for (int i = 0; i < 80; i++) 
		{
			printf("%c", screen[i][j]);
		}
		printf("\n");
	}

	return;
}

// Main
int main(int argc, char *argv[]) 
{

	// Check argument number
	if (argc < 2) 
	{		
		printf("Please provide the model path.\n");
		return 1;	
	}

	// Parse the model
	if (parse_obj(argv[1]))
		return 2;

	float rot = 0;
	
	// Keep looping until user hit 'q'
	do
	{
		// Restore the mesh
		restore();

		// Set scale, rotation and positon
		rotate_y(rot += 0.1);
		translate(0, 0, -5);
		
		// Render it
		draw();

	} while(getchar()!='q');

	// Exit
	return 0;
}