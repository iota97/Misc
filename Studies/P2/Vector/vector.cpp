#include "vector.h"
#include <iostream>
using namespace std;

Vector::Vector() : arr(0), size(0) {}

Vector::~Vector() { delete[] arr; }

Vector::Vector(const Vector& v) : arr(copia_arr(v.arr, v.size)), size(v.size) {}

Vector& Vector::operator=(const Vector& v) { 
	if (this != &v) {
		delete[] arr;
		arr = copia_arr(v.arr, v.size);
		size = v.size;
	}
	return *this; 
}

void Vector::inserisci(int a) {
	int* aux = new int[size+1];
	for (int i = 0; i < size; i++) {
		aux[i] = arr[i];
	}
	aux[size] = a;
	size++;
	delete[] arr;
	arr = aux;
}

int* Vector::copia_arr(int* v, int size) {
	int* aux = new int[size];
	for (int i = 0; i < size; i++) {
		aux[i] = v[i];
	}
	return aux;
}

std::ostream& operator<<(std::ostream& os, const Vector& v) {
	for (int i = 0; i < v.size-1; i++)
		os << v.arr[i] << " ";
	os << v.arr[v.size-1];
	return os;
}

bool Vector::operator==(const Vector& v) const {
	if (size != v.size) return false;
	for (int i = 0; i < size; i++)
		if (arr[i] != v.arr[i]) return false;
	return true;
}

Vector Vector::operator+(const Vector& v) const {
	Vector aux(*this);
	aux.append(v);
	return aux;
}

int& Vector::operator[](int offset) const {
	return arr[offset];
}

int& Vector::operator[](const Iteratore& i) const {
	return *i.p;
}

void Vector::append(const Vector& v) {
	int* aux = new int[size+v.size];
	for (int i = 0; i < size; i++) {
		aux[i] = arr[i];
	}
	for (int i = 0; i < v.size; i++) {
		aux[size+i] = v.arr[i];
	}
	size += v.size;
	delete[] arr;
	arr = aux;
}

Vector::Iteratore& Vector::Iteratore::operator++() {
	++p;
	return *this;
}

bool Vector::Iteratore::operator==(const Iteratore& i) const {
	return p == i.p;
}

bool Vector::Iteratore::operator!=(const Iteratore& i) const {
	return p != i.p;
}

int& Vector::Iteratore::operator*() const {
	return *p;
}

int* Vector::Iteratore::operator->() const {
	return p;
}

Vector::Iteratore Vector::begin() const {
	Iteratore aux;
	aux.p = arr;
	return aux;
}

Vector::Iteratore Vector::end() const {
	Iteratore aux;
	aux.p = arr+size;
	return aux;
}

int main () {
	Vector a;
	a.inserisci(1);
	a.inserisci(2);
	a.inserisci(3);
	a.inserisci(4);
	Vector b(a);
	Vector c;
	c = a;
	c[2] = 15;
	Vector d(a);
	d.append(c);
	d.append(c);

	cout << "a: " << a << endl << "b: " << b << endl << "c: " << c << endl << "d: " << d << endl;
	cout << "a==b: " << (a==b ? "true" : "false") << endl;
	cout << "a==c: " << (a==c ? "true" : "false") << endl;
	cout << "a+c: " << a+c << endl;
	
	for(auto& i : a)
		i += 10;

	cout << a << endl;
	
	Vector::Iteratore i = b.begin();
	++i;
	cout << b[i] << endl;

	return 0;
}
