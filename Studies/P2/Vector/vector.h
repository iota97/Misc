#pragma once
#include <iostream>

class Vector {
public:
	Vector();
	~Vector();
	Vector(const Vector& v);
	Vector& operator=(const Vector& v);
	
	class Iteratore {
	friend class Vector;
	public:
		Iteratore& operator++();
		bool operator==(const Iteratore&) const;
		bool operator!=(const Iteratore&) const;
		int* operator->() const;
		int& operator*() const;
	private:
		int* p;
	};

	void inserisci(int);
	friend std::ostream& operator<<(std::ostream& os, const Vector& v);
	bool operator==(const Vector& v) const;
	Vector operator+(const Vector& v) const;
	int& operator[](int offset) const;
	int& operator[](const Iteratore&) const;
	void append(const Vector& v);
	
	

	Iteratore begin() const;
	Iteratore end() const;
private:
	static int* copia_arr(int*, int);
	int* arr;
	int size;
};
