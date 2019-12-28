// A sphere and a cube with distance based ray marching

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Vertex struct
typedef struct vec3_s
{
	float x;
	float y;
	float z;
} vec3;

// Scene descriptor
struct scene_s
{
	vec3 sphere_center;
	vec3 cube_center;
	float sphere_radius;
	float cube_size;
	int operation;
	int step;
};

static struct scene_s scene;

// Union: min(d0, d1)
float mesh_union(float d0, float d1)
{
	return 	d0 < d1 ? d0 : d1;
}

// Difference: max(d0, -d1)
float mesh_difference(float d0, float d1)
{
	return 	d0 > -d1 ? d0 : -d1;
}

// Intersection max(d0, d1)
float mesh_intersection(float d0, float d1)
{
	return 	d0 > d1 ? d0 : d1;
}

// Sphere distance
float sphere(vec3 ray, vec3 pos, float radius)
{
	float x = ray.x - pos.x;
	float y = ray.y - pos.y;
	float z = ray.z - pos.z;
	float length = sqrt(x*x + y*y + z*z); 
	return length - radius;
}

// Cube distance
float cube(vec3 ray, vec3 pos, float size)
{

	float x = fabs(ray.x - pos.x) - size;
	float y = fabs(ray.y - pos.y) - size;
	float z = fabs(ray.z - pos.z) - size;
	x = x < 0.0 ? 0.0 : x;
	y = y < 0.0 ? 0.0 : y;
	z = z < 0.0 ? 0.0 : z;
	float length = sqrt(x*x + y*y + z*z); 
	return length;
}

// Scene distance
float scene_distance(vec3 pos)
{
	float dist_sphere = sphere(pos, scene.sphere_center, scene.sphere_radius);
	float dist_cube = cube(pos, scene.cube_center, scene.cube_size);

	switch(scene.operation)
	{
		case 0: return mesh_union(dist_sphere, dist_cube);
		case 1: return mesh_difference(dist_sphere, dist_cube);
		case 2: return mesh_difference(dist_cube, dist_sphere);
		case 3: return mesh_intersection(dist_sphere, dist_cube);
	}
}

// Ray cast from pixel
int cast_ray(float x, float y)
{
	vec3 pos;
	pos.x = x;
	pos.y = y;
	pos.z = 0.0;

	float dist = 0.0;
	for (int i = 0; i < scene.step; i++)
	{
		float dist = scene_distance(pos);
		if (dist < 0.000001)
			return 1;

		pos.z += dist;
	}

	return 0;
}

// Print all pixel
void render()
{	
	for (int y = 0; y < 24; y++)
	{
		for (int x = 0; x < 80; x++)
		{
			putchar(cast_ray(x/80.0-0.5, y/40.0-0.3) ? '*' : ' ');	
		}
	}
	return;
}

// Print help message
void print_help()
{
	puts("SYNTAX: ./ray sphere.x sphere.y sphere.z sphere.r cube.x cube.y cube.z cube.s MODE step");
	puts("\t MODE: 0 - union; 1 or 2 - difference; 3 - intersetion");
	puts("Example: ./ray 0.0 0.0 0.0 1.5 0.0 0.0 0.0 1.0 2 64");
	return;
}

// Main
int main(int argc, char* argv[])
{
	// Need 10 argument
	if (argc < 11)
	{
		print_help();
		return 1;
	}	

	// Setup scene
	scene.sphere_center.x = atof(argv[1]);
	scene.sphere_center.y = atof(argv[2]);
	scene.sphere_center.z = atof(argv[3]);
	scene.sphere_radius = atof(argv[4])/10.0;
	scene.cube_center.x = atof(argv[5]);
	scene.cube_center.y = atof(argv[6]);
	scene.cube_center.z = atof(argv[7]);
	scene.cube_size = atof(argv[8])/10.0;
	scene.operation = atoi(argv[9]);
	scene.step = atoi(argv[10]);

	// Render it
	render();

	return 0;
}
