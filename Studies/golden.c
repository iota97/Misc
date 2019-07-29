/*

Floating point golden ratio calculator

Cycle used: 5 for both precision
Correct digit vs internet reference: 6 in single precision, 16 in double
Loop count needed to compute all the correct digit: 4 for both

Newton method formula:

              x*(x^2 - x - 1)
  x0  =   x - _______________

                 x^2 + 1

*/

#include <stdio.h>

// Max newton method iteration
#define MAX_LOOP -1

// Compute the golden ratio with double
double d_golden_ratio()
{
	// Count the loop number
	unsigned int loop_count = 0;

	// Assign 2 as first value
	double golden_ratio = 2.;

	// Assign -1 as a fake first old value
	double old_golden_ratio = -1.;
 
	// Loop MAX_LOOP time or until there is no more difference from the previous iteration
	for (int i = 0; i != MAX_LOOP && golden_ratio != old_golden_ratio; i++)
	{
		// Save golden ratio
		old_golden_ratio = golden_ratio;

		// Compute newton method
		golden_ratio -= golden_ratio * (golden_ratio*golden_ratio - golden_ratio - 1.) /
				(golden_ratio*golden_ratio + 1.);

		// Increase the loop counter
		loop_count++;
	}

	// Print the loop count
	printf("Double precision loop count: %u\n", loop_count);

	// Return the value
	return golden_ratio;
}

// Same with single precision float
float f_golden_ratio()
{
	unsigned int loop_count = 0;
	float golden_ratio = 1.;
	float old_golden_ratio = -1.;
 	
	for (int i = 0; i != MAX_LOOP && golden_ratio != old_golden_ratio; i++)
	{
		old_golden_ratio = golden_ratio;
		golden_ratio -= golden_ratio * (golden_ratio*golden_ratio - golden_ratio - 1.) /
				(golden_ratio*golden_ratio + 1.);
		loop_count++;
	}
	printf("Single precision loop count: %u\n", loop_count);
	return golden_ratio;
}

int main()
{
	// Print first 50 decimal digit
	printf("\nDouble: %.50f\nSingle: %.50f\nOnline: 1.61803398874989484820458683436563811772030917980576\n", 
		d_golden_ratio(), f_golden_ratio());

	return 0;
}