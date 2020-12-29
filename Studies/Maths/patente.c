#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char *argv[]) {
	if (argc < 2) {
		printf("Sintassi: %s <media>\n", argv[0]);
		return 1;
	}
	double media = atof(argv[1]);
	double p = 1.0;
	for (int i = 0; i < 5; ++i) {
		p -= pow(media, i)/(tgamma(i+1)*pow(M_E, media));
	}
	printf("Probabilità bocciatura: %.2f%\n", p*100);
	return 0;
}
