#include <stdio.h>
#include <stdlib.h>

int mcd(int a, int b) {
	// IKR, ma e' tardi e sono pigro...
	return b == 0 ? a : (printf("%d = %d*%d + %d\n", a, b, a/b, a%b), mcd(b, a%b));
}

int main (int argc, char *argv[]) {
	if (argc < 3) {
		puts("Usage: MCD [a] [b]");
		return 1;
	}

	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	a = a > 0 ? a : -a;
	b = b > 0 ? b : -b;

	if (a == 0 && b == 0) {
		puts("MCD = +inf");
	} else {
		int m = mcd(a, b);
		printf("\nMCD = {%d, %d}\n", m, -m);
	}

	return 0;
}
