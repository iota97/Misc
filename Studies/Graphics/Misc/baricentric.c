// Interpola i valori dei vertici di un triangolo in un punto

/*
 non funziona con lati verticali (m = inf)
 il peso di ogni vertice è dato dall'area del triangolo tra punto e gli altri 2 vertici
 l'area deve essere divisa per l'area totale (totale 1)
 essendo una proporzione non serve dividere tutte le aree per 2 (area triangolo = b*h/2)
*/

#include <stdio.h>
#include <math.h>

// struct dei punti
typedef struct point_s
{
	float x;
	float y;
	float val;
} point;

int main()
{
	// a, b, c sono i vertici del triangolo, c il punto
	point a, b, c, p;

	// otteniamo le coordinate del triangolo
	puts("Inserisci i dati del primo vertice come 'x y valore': ");
	scanf("%f %f %f", &a.x, &a.y, &a.val);

	puts("Inserisci i dati del secondo vertice come 'x y valore': ");
	scanf("%f %f %f", &b.x, &b.y, &b.val);

	puts("Inserisci i dati del terzo vertice come 'x y valore': ");
	scanf("%f %f %f", &c.x, &c.y, &c.val);

	// calcoliamo coefficiente angolare, intercetta e lunghezza dei lati
	float m0 = (a.y-b.y)/(a.x-b.x);	
	float q0 = a.y - m0*a.x;
	float l0 = sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));

	float m1 = (b.y-c.y)/(b.x-c.x);	
	float q1 = b.y - m1*b.x;
	float l1 = sqrt((b.x-c.x)*(b.x-c.x)+(b.y-c.y)*(b.y-c.y));

	// area del triangolo, coi segni
	float det1 = 1.0/(l0*(c.y-(m0*c.x+q0))/sqrt(1+m0*m0));
	float det2 = 1.0/(l1*(a.y-(m1*a.x+q1))/sqrt(1+m1*m1));

	// cicliamo fino a un CTRL+C
	for(;;)
	{
		// otteniamo le coordinate del punto
		puts("Insert point coordinate as 'x y': ");
		scanf("%f %f", &p.x, &p.y);

		// calcoliamo la distanza punto retta e la moltipliciamo per la "base"
		float w0 = det1*l0*(p.y-(m0*p.x+q0))/sqrt(1+m0*m0);
		float w1 = det2*l1*(p.y-(m1*p.x+q1))/sqrt(1+m1*m1);
		float w2 = 1.0 - w0 - w1;
		printf("(%f, %f, %f)\n", w0, w1, w2);

		// interpoliamo
		p.val = a.val*w1+b.val*w2+c.val*w0;

		// Print dei risultati
		printf("Valore punto: %f\n", p.val);
	}

	return 0;
}
