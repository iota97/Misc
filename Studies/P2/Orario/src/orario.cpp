#include "orario.h"

orario::orario(int o, int m, int s) {
	if (o < 0 || o > 23 || m < 0 || m > 59 || s < 0 || s > 59) {
		sec = 0;
	} else {
		sec = o * 3600 + m * 60 + s;
	}
}

orario::operator int() const {
	return sec;
}

int orario::Ore() const {
	return sec / 3600;
}

int orario::Minuti() const {
	return (sec / 60) % 60;
}

int orario::Secondi() const {
	return sec % 60;
}

const orario orario::OraDiPranzo() {
	return orario(13, 15);
}

orario orario::operator+(const orario& o) const {
	orario aux;
	aux.sec = sec + o.sec % secInUnGiorno;
	return aux;
}

orario orario::operator-(const orario& o) const {
	orario aux;
	aux.sec = sec >= o.sec ? sec - o.sec : sec - o.sec + secInUnGiorno;
	return aux;
}

bool orario::operator==(const orario& o) const {
	return sec == o.sec;
}

bool orario::operator<(const orario& o) const {
	return sec < o.sec;
}

bool orario::operator>(const orario& o) const {
	return sec > o.sec;
}

std::ostream& operator<<(std::ostream& os, const orario& o) {
	return os << o.Ore() << ':' << o.Minuti() << ':' << o.Secondi();
};
