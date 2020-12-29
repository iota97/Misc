#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char *argv[]) {
	if (argc < 2) {
		printf("Sintassi: %s <media>\n", argv[0]);
		return 1;
	}
	double x = atof(argv[1]);
	double p = (1+x+x*x/2+x*x*x/6+x*x*x*x/24)/pow(M_E, x);
	printf("Probabilità promozione: %.2f%\n", p*100);
	return 0;
}
