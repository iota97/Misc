#include <cstdio>
#include <cstdlib>

class X {
public:
	X(int n = 1, int d = 1, int e = 1) : n(n/gcd(n, d)), d(d/gcd(n, d)), e(e) { }
	int num() const { return n; }
	int den() const { return d; }
	int esp() const { return e; }
	X& operator*=(const X& rhs) {
		int new_n = n * rhs.n/gcd(n * rhs.n, d * rhs.d);
		d = d * rhs.d/gcd(n * rhs.n, d * rhs.d);
		n = new_n;
		reduce();
        	return *this;
	}
	X& operator-=(const X& rhs) {
		int old_d = d;
		n *= rhs.d;
		d *= rhs.d;
		n -= rhs.n*old_d;
		reduce();
        	return *this;
	}
	X& integrate() {
		e++;
		d *= e;
		reduce();
		return *this;
	}
	void reduce() {
		int g = gcd(n, d);
		n /= g;
		d /= g;
		if (d < 0) {
			n = -n;
			d = -d;
		}
	}
private:
	int gcd(int a, int b) {
		return b == 0 ? a : gcd(b, a % b);
	}
	int n, d, e;
};
X operator*(X lhs, const X& rhs) {
	return lhs *= rhs;
}
X operator-(X lhs, const X& rhs) {
	return lhs -= rhs;
}

int pow(int a, int b) {
	int p = 1;
	for (int i = 0; i < b; ++i)
		p *= a;
	return p;
}

int sum(int n, int k) {
	int s = 0;
	for (int i = 1; i <= n; ++i)
		s += pow(i, k);
	return s;
}
 
int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Syntax: %s [power]\n", argv[0]);
		return 1;
	}

	int M = atoi(argv[1])+1;
	X c[256];

	for (int r = 0; r < M; r++) {
   		for (int i = r; i >= 0; i--) {
   			c[i] = c[i-1].integrate() * r;
   		}
   		c[0] = X();
   		for (int i = 1; i <= r; i++) {
   			c[0] -= c[i];
   		}
   		
   		if (r == M-1) {
	   		printf("\nn^%d:  ", r);
	   		for (int i = r; i >= 0; i--) {
	   			if (!c[i].num()) continue;
	   			if (c[i].num() != 1 && c[i].num() != -1) printf("%d", c[i].num());
	   			if (c[i].num() == -1) printf("-");
	   			if (c[i].esp()) printf("X");
	   			if (c[i].esp() > 1) printf("^%d", c[i].esp());
	   			if (c[i].den() != 1) printf("/%d", c[i].den());
	   			printf("  ");
	   		}
	   		
	   		printf("\n\nTest value:\n\n");
	   		for (int j = 0; j < 10; ++j) {
	   			X v = 0;
	   			for (int i = r; i >= 0; i--) {
	   				v -= -pow(j, i+1)*c[i];
	   			}
	   			printf("%d -- %d: %s\n", v.num(), sum(j, r), (v.num() == sum(j, r)) ? "OK!" : "ERROR (overflow?)");
	   		}
	   	}
   	}

	return 0;
}
