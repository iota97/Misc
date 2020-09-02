#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define u64 uint64_t

u64 p(u64 a, u64 b) {
	u64 c = 1;
	for (u64 i = 0; i < b; ++i)
		c *= a;
	return c;
}

u64 s(u64 n, u64 k) {
	u64 a = 0;
	for (u64 i = 0; i <= n; ++i)
		a += p(i, k);
	return a;
}

u64 h(u64 n, u64 k, u64 c[][2]) {
	u64 a = 1;
	u64 d = 1;
	for (u64 i = 0; i <= k; ++i) {
		a *= c[i][0]*n + c[i][1];
		d *= c[i][0] + c[i][1];
	}
	return d ? a/d : -1;
}

int t(u64 k, u64 c[][2], u64 f) {
	for (u64 i = 0; i < f; ++i)
		if (s(i, k) != h(i, k, c))
			return 0;
	return 1;
}

int d(u64 k, u64 c[][2], u64 max, u64 f) {
	for (u64 i = 0; i < p(max, (k+1)*2); ++i) {
		for (u64 j = 0; j < (k+1)*2; ++j)
			c[j/2][j%2] = (i/p(max, j)) % max;
		
		if (t(k, c, f)) {
			for (u64 j = 0; j <= k; ++j) {
				if (c[j][1] != 0)
					printf("(");
				if (c[j][0] != 1)
					printf("%ld", c[j][0]);
				printf("n");
				if (c[j][1] != 0)
					printf("%+ld)", c[j][0], c[j][1]);
			}
			u64 d = 1;
			for (u64 l = 0; l <= k; ++l)
				d *= c[l][0] + c[l][1];
			printf("/%d\n", d);
			return 0;
		}
	}

	printf("No match\n");
	return 1;
}

int main (int argc, char *argv[]) {
	if (argc < 4) {
		printf("Usage: %s [power] [max_coef] [max_n_check]\n", argv[0]);
		return 2;
	}
		
	u64 k = atoi(argv[1]);
	u64 c[k+1][2];
	return d(k, c, atoi(argv[2])+1, atoi(argv[3]));;
}
