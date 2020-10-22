#pragma once
#include <iostream>

class orario {
public:
	orario(int o = 0, int m = 0, int s = 0);
	operator int() const;
	orario operator+(const orario&) const;
	orario operator-(const orario&) const;
	bool operator==(const orario&) const;
	bool operator<(const orario&) const;
	bool operator>(const orario&) const;
	int Ore() const;
	int Minuti() const;
	int Secondi() const;
	static const orario OraDiPranzo();
	friend std::ostream& operator<<(std::ostream&, const orario&);

private:
	int sec;
	const static int secInUnGiorno = 86400;
};
