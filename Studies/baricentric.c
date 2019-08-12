// Interpola i valori dei vertici di un triangolo in un punto

/* 
 non controlla l'appartenenza del punto al triangolo (usa fabs)
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

	float m2 = (c.y-a.y)/(c.x-a.x);	
	float q2 = c.y - m2*c.x;
	float l2 = sqrt((c.x-a.x)*(c.x-a.x)+(c.y-a.y)*(c.y-a.y));

	// cicliamo fino a un CTRL+C
	for(;;)
	{
		// otteniamo le coordinate del punto
		puts("Insert point coordinate as 'x y': ");
		scanf("%f %f", &p.x, &p.y);

		// calcoliamo la distanza punto retta e la moltipliciamo per la "base"
		float w0 = l0*fabs(p.y-(m0*p.x+q0))/sqrt(1+m0*m0);
		float w1 = l1*fabs(p.y-(m1*p.x+q1))/sqrt(1+m1*m1);
		float w2 = l2*fabs(p.y-(m2*p.x+q2))/sqrt(1+m2*m2);

		// interpoliamo
		float sum = w0+w1+w2;
		p.val = (a.val*w1+b.val*w2+c.val*w0)/sum;

		// Print dei risultati
		printf("Valore punto: %f\n", p.val);
	}

	return 0;
}