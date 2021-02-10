#include "TestClass.h"
#include <iostream>

void ref(const int& i) {
	std::cout << i << std::endl;
}

void val(int i) {
	std::cout << i << std::endl;
}

int main (int argc, char *argv[]) {
	TestClass c;

	ref(c.anche_nel_cpp);
	//ref(c.solo_nel_h); // LINKING ERROR!

	val(c.anche_nel_cpp);
	val(c.solo_nel_h);

	return 0;
}
