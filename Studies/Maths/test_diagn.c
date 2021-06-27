#include <stdio.h>
#include <stdlib.h>

float pos(float sens, float spec, float prev) {
	return (sens*prev)/(sens*prev+(1-spec)*(1-prev));
}

int main (int argc, char *argv[]) {
	if (argc < 4) {
		printf("Sintassi: %s [sensibilita] [specificita] [prevalanze]\n", argv[0]);
		exit(1);
	}
	printf("Prob. malato dato un test positivo: %.2f%\n", 100.0f*pos(atof(argv[1]), atof(argv[2]), atof(argv[3]))); 
	return 0;
}
