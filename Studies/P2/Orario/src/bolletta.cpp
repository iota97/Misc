#include "bolletta.h"

bolletta::bolletta() : first(0) {}

bolletta::bolletta(const bolletta& b) : first(copia(b.first)) {}

bolletta::~bolletta() { distruggi(first); }

bool bolletta::Vuota() const {
	return !first;
}

void bolletta::AggiungiTelefonata(const telefonata& t) {
	first = new nodo(t, first);
}

void bolletta::TogliTelefonata(const telefonata& t) {
	nodo* p = first;
	nodo* prec = 0;
	while (p && !(p->info == t)) {
		prec = p;
		p = p->next;
	}
	if (p) {
		if (!prec) {
			first = p->next;
		} else {
			prec->next = p->next;
		}
		delete p;
	}
}

// Se non vuota
telefonata bolletta::EstraiUna() {
	nodo* p = first;
	first = first->next;
	telefonata aux = p->info;
	delete p;
	return aux;
}

bolletta::nodo::nodo() : next(0) {}
bolletta::nodo::nodo(const telefonata& t, nodo* s) : info(t), next(s) {}

std::ostream& operator<<(std::ostream& o, const bolletta& b) {
	bolletta::nodo* p = b.first;
	int i = 1;
	while (p) {
		o << i++ <<") " << p->info;
		if (p->next)
			o << std::endl;
		p = p->next;
	}
	return o;
}

bolletta::nodo* bolletta::copia(nodo* p) {
	if (!p) return 0;
	nodo* primo = new nodo(p->info, 0);
	nodo* q = primo;
	while(p->next) {
		p = p->next;
		q->next = new nodo(p->info, 0);
		q = q->next;
	}
	return primo;
}

void bolletta::distruggi(nodo* p) {
	while (p) {
		nodo* q = p;
		p = p->next;
		delete q;
	}
}

bolletta& bolletta::operator=(const bolletta& y) {
	if (this != &y) {
		distruggi(first);
		first = copia(y.first);
	}
	return *this;
}

orario Somma_Durate(bolletta b) {
	orario durata;
	while (!b.Vuota()) {
		telefonata t = b.EstraiUna();
		durata = durata + t.Fine() - t.Inizio();
	}
	return durata;
}

bolletta Chiamate_A(bolletta& b, int num) {
	bolletta selezionate, resto;
	while (!b.Vuota()) {
		telefonata t = b.EstraiUna();
		if (t.Numero() == num) {
			selezionate.AggiungiTelefonata(t);
		} else {
			resto.AggiungiTelefonata(t);
		}
	}
	b = resto; // Ripristino dopo
	return selezionate;
}

bool bolletta::iteratore::operator==(const iteratore& i) const {
	return punt == i.punt;
}

bool bolletta::iteratore::operator!=(const iteratore& i) const {
	return punt != i.punt;
}

bolletta::iteratore& bolletta::iteratore::operator++() {
	if (punt) punt = punt->next;
	return *this;
}

bolletta::iteratore bolletta::iteratore::operator++(int) {
	iteratore aux = *this;
	if (punt) punt = punt->next;
	return aux;
}

bolletta::iteratore bolletta::begin() const {
	iteratore aux;
	aux.punt = first;
	return aux;
}

bolletta::iteratore bolletta::end() const {
	iteratore aux;
	aux.punt = nullptr;
	return aux;
}

telefonata& bolletta::operator[](const iteratore& it) const {
	return it.punt->info;
}

telefonata& bolletta::iteratore::operator*() const {
	return punt->info;
}

telefonata* bolletta::iteratore::operator->() const {
	return &(punt->info);
}

