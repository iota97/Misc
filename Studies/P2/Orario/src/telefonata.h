#pragma once
#include "orario.h"

class telefonata {
public:
	telefonata();
	telefonata(const orario& i, const orario& f, int n);
	friend std::ostream& operator<<(std::ostream&, const telefonata&);
	bool operator==(const telefonata&) const;
	orario Inizio() const;
	orario Fine() const;
	int Numero() const;
	
private:
	orario inizio;
	orario fine;
	int numero;
};
