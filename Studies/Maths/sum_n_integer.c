#include <stdio.h>
#include <stdlib.h>

void n1(int n) {
	int s = 0;
	for (int i = 1; i <= n; ++i)
		s += i;
	
	int v = (n+1)*n/2;

	printf("n^1: %d%s\n", s, s-v ? " ERROR!" : "");
	return;
}

void n2(int n) {
	int s = 0;
	for (int i = 1; i <= n; ++i)
		s += i*i;
	
	int v = (2*n+1)*(n+1)*n/6;

	printf("n^2: %d%s\n", s, s-v ? " ERROR!" : "");
	return;
}

int main (int argc, char *argv[]) {
	int n = atoi(argv[1]);
	
	n1(n);
	n2(n);

	return 0;
}
