#pragma once
#include "telefonata.h"

class bolletta {
private:
	class nodo {
	public:
		nodo();
		nodo(const telefonata&, nodo*);
		telefonata info;
		nodo* next;
	};
	
	static nodo* copia(nodo*);
	static void distruggi(nodo*);
	nodo* first;
public:
	bolletta();
	bolletta(const bolletta&);
	~bolletta();
	bool Vuota() const;
	void AggiungiTelefonata(const telefonata&);
	void TogliTelefonata(const telefonata&);
	telefonata EstraiUna();
	
	bolletta& operator=(const bolletta& y);
	friend std::ostream& operator<<(std::ostream&, const bolletta&);
	
	class iteratore {
		friend class bolletta;
	public:
		bool operator==(const iteratore &) const;
		bool operator!=(const iteratore &) const;
		iteratore& operator++();
		iteratore operator++(int);
		telefonata& operator*() const;
		telefonata* operator->() const;
	private:
		bolletta::nodo* punt;
	};
	iteratore begin() const;
	iteratore end() const;
	telefonata& operator[](const iteratore&) const;
};

orario Somma_Durate(bolletta b);
bolletta Chiamate_A(bolletta& b, int num);
