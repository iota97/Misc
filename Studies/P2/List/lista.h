#ifndef LISTA_H
#define LISTA_H
template<class T>
class dList {
private:
	class nodo {
	public:
		T info;
		nodo *prev, *next;
		nodo(const T& t, nodo *p = 0, nodo *n = 0);
	};
	nodo *last, *first;

	static nodo* copia(nodo* n, nodo* &l, nodo* p = 0);
	static void distruggi(nodo* n);
public:
	dList(unsigned int k, const T& t);
	// Ordine di dichiarazione: nodo *last, *first;
	dList(const dList& l);
	dList& operator=(const dList& l);
	~dList();
	void insert_front(const T& t);
	void insert_back(const T& t);

	bool operator<(const dList& l) const;

	class const_iterator {
	friend class dList;
	private:
		const nodo* ptr;
		bool end;
		const_iterator(nodo *p, bool e);
	public:
		const_iterator();
		const_iterator& operator++();
		const_iterator& operator--();
		const_iterator operator++(int);
		const_iterator operator--(int);
		const T& operator*() const;
		const T* operator->() const;
		bool operator==(const const_iterator& it) const;
		bool operator!=(const const_iterator& it) const;
	};

	const_iterator begin();
	const_iterator end();
};

template<class T>
typename dList<T>::const_iterator dList<T>::begin() {
	return const_iterator(first, !first);
}

template<class T>
typename dList<T>::const_iterator dList<T>::end() {
	return const_iterator(last, true);
}

template<class T>
dList<T>::const_iterator::const_iterator(nodo *p, bool e) : ptr(p), end(e) {}

template<class T>
dList<T>::const_iterator::const_iterator() : ptr(0), end(0) {}


template<class T>
const T& dList<T>::const_iterator::operator*() const {
	return ptr->info;
}

template<class T>
const T* dList<T>::const_iterator::operator->() const {
	return &(ptr->info);
}

template<class T>
bool dList<T>::const_iterator::operator==(const const_iterator& it) const {
	return (end == it.end) && (ptr == it.ptr);
}

template<class T>
bool dList<T>::const_iterator::operator!=(const const_iterator& it) const {
	return !(*this == it);
}

template<class T>
typename dList<T>::const_iterator& dList<T>::const_iterator::operator--() {
	if (!end) {
		ptr = ptr->prev;
	} else {
		end = false;
	}
	return *this;
}

template<class T>
typename dList<T>::const_iterator& dList<T>::const_iterator::operator++() {
	if (ptr->next) {
		ptr = ptr->next;
	} else {
		end = true;
	}
	return *this;
}

template<class T>
typename dList<T>::const_iterator dList<T>::const_iterator::operator++(int) {
	const_iterator aux(*this);
	++*this;
	return aux;
}

template<class T>
typename dList<T>::const_iterator dList<T>::const_iterator::operator--(int) {
	const_iterator aux(*this);
	--*this;
	return aux;
}

template<class T>
dList<T>::nodo::nodo(const T& t, nodo *p /* default: 0 */, nodo *n /* default: 0 */) : info(t), prev(p), next(n) {}

template<class T>
dList<T>::dList(unsigned int k, const T& t) {
	if (k >= 1) {
		nodo *aux = first = new nodo(t);
		for (int i = 1; i < k; ++i) {
			aux->next = new nodo(t, aux);
			aux = aux->next;
		}
		last = aux;
	} else {
		first = last = 0;
	}
}

template<class T>
dList<T>::dList(const dList& l) : first(copia(l.first, last)) {}

template<class T>
typename dList<T>::nodo* dList<T>::copia(nodo* n, nodo* &l, nodo* p /* default: 0 */) {
	if (!n) {
		l = p;
		return 0;
	}
	nodo* aux = new nodo(n->info, p);
	aux->next = copia(n->next, l, aux);
	return aux;
}

template<class T>
dList<T>& dList<T>::operator=(const dList& l) {
	if (this != &l) {
		distruggi(first);
		first = copia(l.first, last);
	}
	return *this;
}

template<class T>
dList<T>::~dList() {
	distruggi(first);
}

template<class T>
void dList<T>::distruggi(nodo* n) {
	while (n) {
		nodo* aux = n->next;
		delete n;
		n = aux;
	}
}

template<class T>
bool dList<T>::operator<(const dList& l) const {
	if (this == &l) return false;

	nodo *a1 = first;
	nodo *a2 = l.first;
	while (a2) {
		if (!a1 || a1->info < a2->info)
			return true;
		a1 = a1->next;
		a2 = a2->next;
	}
	return false;
}

template<class T>
void dList<T>::insert_front(const T& t) {
	if (first) {
		first->prev = new nodo(t, 0, first);
		first = first->prev;
	} else {
		first = last = new nodo(t, 0, 0);
	}
}

template<class T>
void dList<T>::insert_back(const T& t) {
	if (last) {
		last->next = new nodo(t, last, 0);
		last = last->next;
	} else {
		first = last = new nodo(t, 0, 0);
	}
}
#endif
