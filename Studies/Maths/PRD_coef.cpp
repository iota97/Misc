#include <cstdio>
#include <cstdlib>

class MonteCarlo {
public:
	MonteCarlo(double c, int i = 1000000) : m_c(c), m_k(1), m_i(i) {}

	double Odd() {
		int acc = 0;
		for (int i = 0; i < m_i; ++i) 
		acc += Roll();

		return (double)acc/m_i;
	}

private:
	bool Roll() {
		bool res = (float)rand()/(float)(RAND_MAX) < m_c*m_k;
		m_k = res ? 1 : m_k+1;
		return res;
	}

	int m_k;
	int m_i;
	double m_c;
};

class Bisect {
public:
	Bisect(double odd, double eps = 0.0001) : m_odd(odd), m_eps(eps) {}
	double C() {
		double c0 = 0.0;
		double c1 = 1.0;
		for (;;) {
			double cm = (c0+c1)*0.5;
			double odd = MonteCarlo(cm).Odd();
			if (odd < m_odd - m_eps) {
				c0 = cm;
			} else if (odd > m_odd + m_eps) {
				c1 = cm;
			} else {
				return cm;
			}
		}
	}

private:
	double m_odd;
	double m_eps;
};

int main (int argc, char *argv[]) {
	if (argc < 2) {
		printf("Synthax: %s [target]\n", argv[0]);
		return -1;
	}
	printf("%f\n", Bisect(atof(argv[1])).C());

	return 0;
}
