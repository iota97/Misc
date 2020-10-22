#pragma once
#include "telefonata.h"

class bolletta {
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
};

orario Somma_Durate(bolletta b);
bolletta Chiamate_A(bolletta& b, int num);
