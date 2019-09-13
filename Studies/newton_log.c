/* 
Test newton method on Log (not a good idea)
	
Convergence is REALLY slow
Return only integer value for X > 2^15 (2^8 with single precision)
2^x with X < 1 give -NaN
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[])
{
	// Read argument
	union K	{ double f; long int i;} k;
	k.f = atof(argv[1]);

	// Use exponent as a starting point
	double x = (((k.i&0x7FF0000000000000) >> 52)-1023);
	
	long long int c = 0;
	double old_x;
	
	// Newton method loop
	do 
	{
		old_x = x;
		x = x - (pow(2, x) - k.f)/pow(2*x, x-1);
		c++;
		
	} while (x != old_x);

	// Get math.h result
	double m = log2(k.f);
	double d = fabs(m-x);

	// Log result
	printf("Newton:\t\t%.49f\nMath.h:\t\t%.49f\nDelta:\t\t%.49f\nCycle count:\t%d\n", x, m, d,c);
	return 0;
}
