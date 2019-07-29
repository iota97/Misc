// Custom sine and cosine accuracy test

// Error: 0.000523%

#define START -5000.f
#define END 5000.f
#define INCREMENT 0.001f
#define MAX_LOOP -1

#include <stdio.h>

#define PI 3.14159265359f

// Normalize rotation between 0 and 2PI
float normalized_angle(float x)
{
	if (x < 0)
		x += 2*PI*((int)(-x/(2*PI))+1);
	else 
		x -= 2*PI*(int)(x/(2*PI));

	return x;
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

// Calculate the absolute value
float absolute(float x)
{
	return x > 0 ? x : -x;
}

// Main loop
int main()
{
	double i = START;
	double err = 0.;
	int j = 0;

	while (i < END && j != MAX_LOOP)
	{
		err += absolute(1.0-(sine(i)*sine(i)+cosine(i)*cosine(i)));
		i += INCREMENT;
		j++;
	}

	// Print % error
	printf("Error: %lf%\n", (err*100/j));
	return 0;
}
