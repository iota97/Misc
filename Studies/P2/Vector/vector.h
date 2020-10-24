#pragma once
#include <iostream>

class Vector {
public:
	Vector();
	~Vector();
	Vector(const Vector& v);
	Vector& operator=(const Vector& v);
	void inserisci(int);
	friend std::ostream& operator<<(std::ostream& os, const Vector& v);
	
	bool operator==(const Vector& v) const;
	Vector operator+(const Vector& v) const;
	int& operator[](int offset) const;
	void append(const Vector& v);
private:
	static int* copia_arr(int*, int);
	int* arr;
	int size;
};
