/* Advent of code 2019 - 4 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	/* Parse input */
	if (argc < 2)
	{
		puts("Syntax error: Use program [start-stop]");
		return 1;
	}
	int start, stop;
	sscanf(argv[1], "%d-%d", &start, &stop);

	int count = 0;
	int count2 = 0;
	
	/* Cycle all value */
	for (int n = start; n <= stop; n++)
	{
		/* Get digit */
		int d6 = (n/100000)%10;
		int d5 = (n/10000)%10;
		int d4 = (n/1000)%10;
		int d3 = (n/100)%10;
		int d2 = (n/10)%10;
		int d1 = (n/1)%10;

		/* First part check */
		if (d6 > d5 || d5 > d4 || d4 > d3 || d3 > d2 || d2 > d1)
			continue;
		if (d6 != d5 && d5 != d4 && d4 != d3 && d3 != d2 && d2 != d1)
			continue;
		count++;

		/* Second part check */
		if ((d1 == d2 && d2 != d3) ||  (d1 != d2 && d2 == d3 && d3 != d4) || 
		(d2 != d3 && d3 == d4 && d4 != d5) || (d3 != d4 && d4 == d5 && d5 != d6) ||
		(d4 != d5 && d5 == d6))
		count2++;
	}

	/* Print result and exit */
	printf("%d, %d\n", count, count2);
	return 0;
}
