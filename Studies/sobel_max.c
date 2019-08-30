// Bruteforce sobel max magnitude

#include <stdio.h>
#include <math.h>

/*
	0 0 0
	0 1 1
	1 1 1

	x = 1+2+1 = 4
	y = 0+2+0 = 2

	sqrt(16+4) = 4.472136
*/

int main()
{
	double max = 0;
	int pattern;
	double angle;

	for (int i = 0; i < 255; i++)
	{
		double x = ((i >> 0)&1)+2*((i >> 1)&1)+((i >> 2)&1)-((i >> 5)&1)-2*((i >> 6)&1)-((i >> 7)&1);
		double y = ((i >> 2)&1)+2*((i >> 4)&1)+((i >> 7)&1)-((i >> 0)&1)-2*((i >> 3)&1)-((i >> 5)&1);

		double mag = sqrt(x*x+y*y);

		if (mag > max)
		{
			max = mag;
			pattern = i;
			angle = atan(y/x)*(180/M_PI);
		}
	}

	printf("Magnitude: %f, Angle: %.2f, Pattern: %d\n", max, angle, pattern);
	return 0;
}
