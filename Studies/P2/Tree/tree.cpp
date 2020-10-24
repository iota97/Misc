#include "tree.h"
#include <iostream>
using namespace std;

Nodo::Nodo(char c, Nodo* s, Nodo* d) : info(c), sx(s), dx(d) {}

Tree::Tree() : root(0) {}

Tree::~Tree() { distruggi(root); }

Tree::Tree(const Tree& t) : root(copia(t.root)) {}

Tree& Tree::operator=(const Tree& t) {
	if (this != &t) {
		distruggi(root);
		root = copia(t.root);
	}
	return *this;
}

void Tree::distruggi(Nodo *n) {
	if (!n) return;
	distruggi(n->sx);
	distruggi(n->dx);
	delete n;
}

Nodo* Tree::copia(Nodo *n) {
	if (!n) return 0;
	Nodo* aux = new Nodo(n->info);
	aux->sx = copia(n->sx);
	aux->dx = copia(n->dx);
	return aux;	
}

int main () {
	Tree a;
	Tree b = a;
	Tree c;
	c = a;

	return 0;
}
