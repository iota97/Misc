#include "IZod.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

int main (int argc, char* argv[]) {

	IZod i;

	Wav w("Wav", 32, 100, true);
	Mp3 m("Mp3", 64, 200);
	Mp3 n("Mp3", 32, 300);
	Mp3 o("Mp3", 16, 50);

	i.insert(&m);
	i.insert(&m);
	i.insert(&n);
	i.insert(&o);

	auto x = i.mp3(10, 10);
	auto y = i.braniQual();

	for (auto& r : x) {
		cout << r.getBitRate() << " ";
	}
	cout << endl;

	for (auto& r : y) {
		cout << r->getSize() << " ";
	}
	cout << endl;

	return 0;
}
