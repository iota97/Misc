#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int c, d;
	int e = b;
	int s = 1;

	do {
		s *= 10;
		c = b;
		d = a*b;
		b = 10*(d%s)+e;

	} while (d*10/s != e);
	
	printf("%d is %d parasitic: %d*%d = %d\n", c, a, c, a, c*a);

	return 0;
}
