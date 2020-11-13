#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "lista.h"
using std::cout;
using std::cin;
using std::endl;

int main (int argc, char* argv[]) {

	dList<int> l1(3, 0);
	l1.insert_front(-1);
	l1.insert_front(-2);
	l1.insert_front(-3);
	l1.insert_back(1);
	l1.insert_back(2);
	l1.insert_back(3);

	dList<int> l2(l1);

	l2 = l1;

	auto it = l1.begin();
	auto end = l1.end();

	for (; it != end; it++) {
		cout << *it << endl;
	}

	cout << "---" << endl;

	cout << (l1 < l2) << endl;

	cout << "---" << endl;

	it = --l2.end();
	end = l2.begin();
	for (; it != end; it--)
		cout << *it << endl;
	cout << *it << endl;

	cout << "---" << endl;

	it = l2.end();
	--it;
	cout << *it << endl;

	cout << "---" << endl;

	struct s {
		int v;
	};
	s c;
	c.v = 5;
	dList<s> l3(1, c); 
	auto i = l3.begin();
	cout << i->v << endl;

	return 0;


}
