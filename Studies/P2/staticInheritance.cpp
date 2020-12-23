// Variabile statica separata per ogni sottoclasse

#include <iostream>

// Base effettiva
class Base {
public:
	virtual void foo() {
		std::cout << "Base" << std::endl;
	}
};

// Base intermedia, aggiungi il campo statico
// NB: i membri statici sono separati per ogni istanza
// del template
template<class D>
class BaseStatica : public Base {
public:
	static int x;
};

// I membri statici si inizializzano solo fuori...
template<class D>
int BaseStatica<D>::x = 0;

// Crea una classe derivata che eredita il template istanziato
// con se stessa
class Derivata1 : public BaseStatica<Derivata1> {
	public:
	virtual void foo() {
		std::cout << "Derivata1: " << x++ << std::endl;
	}
};

// Ne creai un altra che eredita un template con se stessa
// cosi sono 2 template separati
class Derivata2 : public BaseStatica<Derivata2> {
	public:
	virtual void foo() {
		std::cout << "Derivata2: " << x++ << std::endl;
	}
};

int main (int argc, char *argv[]) {
	Base b;
	Derivata1 d1;
	Derivata2 d2;
	b.foo();
	d1.foo();
	d1.foo();
	d2.foo();
	d2.foo();
	return 0;
}
