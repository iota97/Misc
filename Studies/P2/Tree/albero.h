#include <iostream>

template <class T>
class albero;

template <class T>
std::ostream& operator<<(std::ostream&, const albero<T>&);

template <class T>
class albero {
private:
	class nodo {
	public:
		T info;
		nodo* sx;
		nodo* cx;
		nodo* dx;
		nodo(const T& x, nodo* s=0, nodo* c=0, nodo* d=0);
		friend std::ostream& operator<< <T>(std::ostream&, const albero<T>&);
	};
	nodo* root;
	
	static nodo* copia(nodo* r);
	static void distruggi(nodo* r);
	static bool uguale(nodo* a, nodo* b);
	static bool cerca(nodo* r, const T& a);

public:
	albero();
	albero(const albero& a);
	albero& operator=(const albero& a);
	~albero();
	bool operator==(const albero& a) const;
	void insert(const T& x);
	bool search(const T& x);

	friend std::ostream& operator<< <T>(std::ostream&, const albero<T>&);
};

template <class T>
std::ostream& operator<<(std::ostream& o, const albero<T>& a) {
	return o << "TODO";
}

template <class T>
bool albero<T>::operator==(const albero& a) const {
	return this == &a || uguale(root, a.root);
}

template <class T>
bool albero<T>::uguale(nodo* a, nodo* b) {
	if (!a && !b) return true;
	if (!a) return false;
	if (!b) return false;
	return a->info == b->info && uguale(a->sx, b->sx) && uguale(a->cx, b->cx) && uguale(a->dx, b->dx);
}

template <class T>
bool albero<T>::cerca(nodo* r, const T& a) {
	if (!r) return false;
	return r->info == a || cerca(r->sx, a) || cerca(r->cx, a) || cerca(r->dx, a);
}

template <class T>
void albero<T>::insert(const T& x) {
	root = new nodo(x, copia(root), copia(root), root);
}

template <class T>
bool albero<T>::search(const T& x) {
	return cerca(root, x);
}

template <class T>
albero<T>::albero() : root(0) {}

template <class T>
albero<T>::albero(const albero& a) : root(copia(a.root)) {}

template <class T>
albero<T>& albero<T>::operator=(const albero& a) {
	if (this != &a) {
		distruggi(root);
		root = copia(a.root);
	}
	return *this;
}

template <class T>
albero<T>::~albero() { distruggi(root); };

template <class T>
typename albero<T>::nodo* albero<T>::copia(nodo* r) {
	if (!r) return 0;
	return new nodo(r->info, copia(r->sx), copia(r->cx), copia(r->dx));
}

template <class T>
void albero<T>::distruggi(nodo* r) {
	if (!r) return;
	distruggi(r->sx);
	distruggi(r->cx);
	distruggi(r->dx);
	delete r;
}

template <class T>
albero<T>::nodo::nodo(const T& x, nodo* s, nodo* c, nodo* d) : info(x), sx(s), cx(c), dx(d) {}


