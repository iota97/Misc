#include <stdio.h>
#include <stdlib.h>

void print_shift(int in)
{
	/* Inline ASM */
	int sar, sal, shr, shl;
	asm ("sar $1, %1; movl %1, %0;" : "=r" (sar): "r" (in));
	asm ("sal $1, %1; movl %1, %0;" : "=r" (sal): "r" (in));
	asm ("shr $1, %1; movl %1, %0;" : "=r" (shr): "r" (in));
	asm ("shl $1, %1; movl %1, %0;" : "=r" (shl): "r" (in));

	/* Print result */
	printf("sar: %d, sal: %d, shr: %d, shl: %d\n", sar, sal, shr, shl);
}

int main(int argc, char **argv)
{
	/* Parse and convert first argument */
	if (argc < 2)
		return 1;

	print_shift(atoi(argv[1]));
	return 0;
}
