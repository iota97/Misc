#pragma once

class Nodo {
friend class Tree;
private:
	Nodo(char c='*', Nodo* s=0, Nodo* d=0);
	char info;
	Nodo* sx;
	Nodo* dx;
};

class Tree {
public:
	Tree();
	~Tree();
	Tree(const Tree& t);
	Tree& operator=(const Tree& t);
private:
	static void distruggi(Nodo *n);
	static Nodo* copia(Nodo *n);
	Nodo* root;
};
