#include "telefonata.h"

telefonata::telefonata() : numero(0) {};
telefonata::telefonata(const orario& i, const orario& f, int n) : inizio(i), fine(f), numero(n) {};

orario telefonata::Inizio() const { return inizio; }
orario telefonata::Fine() const { return fine; }
int telefonata::Numero() const { return numero; }

std::ostream& operator<<(std::ostream& os, const telefonata& t) {
	return os << t.numero << " " << t.inizio << " " << t.fine;
};

bool telefonata::operator==(const telefonata& t) const {
	return t.inizio == inizio && t.fine == fine && t.numero == numero;
}
