// Dump some info on double precision floating point number passed as argument

#include <stdio.h>
#include <stdint.h>
#include <math.h>

int main (int argc, char* argv[]) {

	if (argc < 2) {
		printf("Error: no argument.\n");
		return 1;
	}

	union number {
		double f;
		int64_t i;
	} num;

	if (!sscanf(argv[1], "%lf",  &num.f)) {
		printf("Error: invalid float.\n");
		return 2;
	}

	printf("Bin: ");

	for (int i = 0; i < 64; i++) {

		printf("%ld", (num.i >> (63 - i)) & 1);
		
		if (i == 0 | i == 11)
			printf(" ");
	}

	int32_t exp = ((num.i >> 52) & 0x7ff) - 1023; 

	printf("\nVal: %.128lg\nExp: %d\n2^E: %lg\n", num.f, exp, pow(2., (double) exp));

	return 0;
}
