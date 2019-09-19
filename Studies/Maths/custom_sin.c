// Custom sine and cosine accuracy test

// cc custom_sin.c -O2 -o test -lm -lpthread && ./test

/*
PI 3.14159265359 result

SMALL SINE -- Max: 0.003655% (-11.781015), Avg: 0.000252%
SMALL COSINE -- Max: 0.003652% (-10.210200), Avg: 0.000252%
SINE -- Max: 198.935837% (106667408.000000), Avg: 66.297116%
COSINE -- Max: 200.000000% (85563208.000000), Avg: 94.841570%

*/

/*
PI 3.14159265358979323846

SMALL COSINE -- Max: 0.003645% (-0.785422), Avg: 0.000249%
SMALL SINE -- Max: 0.003643% (11.780985), Avg: 0.000248%
SINE -- Max: 0.003656% (-2000234752.000000), Avg: 0.000249%
COSINE -- Max: 0.003659% (1487533056.000000), Avg: 0.000249%

*/

#include <stdio.h>
#include <math.h>
#include <pthread.h>

//#define PI 3.14159265359f
#define PI 3.14159265358979323846

// Normalize rotation between 0 and 2PI
float normalized_angle(float x)
{
	return (x < 0) ? x + 2*PI*((int)(-x/(2*PI))+1) : x - 2*PI*(int)(x/(2*PI));
}	

// Sine
float sine(float x)
{
	// Normalize the rotation
	x = normalized_angle(x);
		
	// Check sign
	float sign;
	if (x < PI)
		sign = 1;
	else
		sign = -1;

	// Check symmetry
	if (x < PI/2 || (x >= PI && x < 3*PI/2))
		x -= (PI/2)*(int)(x/(PI/2));
	else
		x = PI/2 - x + (PI/2)*(int)(x/(PI/2));

	// Check it to be below 45 degree
	if (x < PI/4)
	{
		// Polinomial approximation
		float x2 = x*x;
		float x3 = x2*x;
		float sine = x - x3/6 + x2*x3/120;

		return sine*sign;
	}
	else
	{
		// Transform to cosine
		x = PI/2 - x;

		// Polinomial approximation
		float x2 = x*x;
		float x4 = x2*x2;
		float x6 = x2*x4;
		float cosine = 1 - x2/2 + x4/24 - x6/720;

		return cosine*sign;
	}
}

// Cosine
float cosine(float x)
{
	// Normalize the rotation
	x = normalized_angle(x);
		
	// Check sign
	float sign;
	if (x < PI/2 || x >= 3*PI/2)
		sign = 1;
	else
		sign = -1;

	// Check symmetry
	if (x < PI/2 || (x >= PI && x < 3*PI/2))
		x -= (PI/2)*(int)(x/(PI/2));
	else
		x = PI/2 - x + (PI/2)*(int)(x/(PI/2));

	// Check it to be below 45 degree
	if (x < PI/4)
	{
		// Polinomial approximation
		float x2 = x*x;
		float x4 = x2*x2;
		float x6 = x2*x4;
		float cosine = 1 - x2/2 + x4/24 - x6/720;

		return cosine*sign;
	}
	else
	{
		// Transform to sine
		x = PI/2 - x;

		// Polinomial approximation
		float x2 = x*x;
		float x3 = x2*x;
		float sine = x - x3/6 + x2*x3/120;

		return sine*sign;
	}
}

void *reduced_test_sin()
{
	// Start reduced test
	printf("Running test on SMALL SINE sample...\n");
	
	double avg_err = 0.;
	double max_err = 0.;
	unsigned long j = 0;
	float max_err_value;

	// Test only few value
	for (float n = -4*PI; n < 4*PI; n+=0.000001)
	{
		
		double err = fabs(sin(n)-sine(n));
		avg_err += err;

		if (err > max_err) 
		{
			max_err = err;	
			max_err_value = n;
		}
		j++;
	}

	// Print errors
	printf("SMALL SINE -- Max: %lf%% (%f), Avg: %lf%\n", max_err*100., max_err_value, (avg_err*100/j));

	return NULL;
}

void *reduced_test_cos()
{
	// Start reduced test
	printf("Running test on SMALL COSINE sample...\n");
	
	double avg_err = 0.;
	double max_err = 0.;
	unsigned long j = 0;
	float max_err_value;

	// Test only few value
	for (float n = -4*PI; n < 4*PI; n+=0.000001)
	{
		
		double err = fabs(cos(n)-cosine(n));
		avg_err += err;

		if (err > max_err) 
		{
			max_err = err;	
			max_err_value = n;
		}
		j++;
	}

	// Print errors
	printf("SMALL COSINE -- Max: %lf%% (%f), Avg: %lf%\n", max_err*100., max_err_value, (avg_err*100/j));

	return NULL;
}

void *sin_test()
{
	// Start sin test
	printf("Running test on Sine...\n");
	
	double avg_err = 0.;
	double max_err = 0.;
	unsigned long j = 0;
	float max_err_value;

	// Test all float32 value
	for (int s = 0; s < (1 << 1); s++)
	{
		for (int e = 0; e < (1 << 8)-1; e++)
		{	
			for (int m = 0; m < (1 << 23); m++)
			{

				float n = (s << 31) | (e << 23) | m;
		
				double err = fabs(sin(n)-sine(n));
				avg_err += err;

				if (err > max_err) 
				{
					max_err = err;	
					max_err_value = n;
				}

				j++;
			}
		}
	}

	// Print errors
	printf("SINE -- Max: %lf%% (%f), Avg: %lf%\n", max_err*100., max_err_value, (avg_err*100/j));

	return NULL;
}

void *cos_test()
{
	// Start cos test
	printf("Running test on Cosine...\n");

	double avg_err = 0.;
	double max_err = 0.;
	unsigned long j = 0;
	float max_err_value;

	// Test all float32 value
	for (int s = 0; s < (1 << 1); s++)
	{
		for (int e = 0; e < (1 << 8)-1; e++)
		{	
			for (int m = 0; m < (1 << 23); m++)
			{

				float n = (s << 31) | (e << 23) | m;
		
				double err = fabs(cos(n)-cosine(n));
				avg_err += err;

				if (err > max_err) 
				{
					max_err = err;	
					max_err_value = n;
				}

				j++;
			}
		}
	}

	// Print errors
	printf("COSINE -- Max: %lf%% (%f), Avg: %lf%\n", max_err*100., max_err_value, (avg_err*100/j));

	return NULL;
}

// Main
int main()
{
	// Run 3 additional thread 
	pthread_t sin_thread, cos_thread, reduced_sin_thread, reduced_cos_thread;

	pthread_create(&sin_thread, NULL, sin_test, NULL);
	pthread_create(&cos_thread, NULL, cos_test, NULL);
	pthread_create(&reduced_sin_thread, NULL, reduced_test_sin, NULL);
	pthread_create(&reduced_cos_thread, NULL, reduced_test_cos, NULL);

	pthread_join(sin_thread, NULL);
	pthread_join(cos_thread, NULL);
	pthread_join(reduced_sin_thread, NULL);
	pthread_join(reduced_cos_thread, NULL);

	return 0;
}
