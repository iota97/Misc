#include <iostream>
#include "orario.h"
#include "telefonata.h"
#include "bolletta.h"

using std::cout;
using std::endl;

int main (int argc, char *argv[]) {
	bolletta bol;
	telefonata t1;
	telefonata t2(orario(7, 30), orario(8), 123);
	telefonata t3(orario(6, 30), orario(9), 456);
	bol.AggiungiTelefonata(t1);
	bol.AggiungiTelefonata(t2);
	bol.AggiungiTelefonata(t3);
	
	
	bolletta bol2(bol);
	
	bol.TogliTelefonata(t3);
	
	cout << bol << endl;
	cout << "---" << endl;
	cout << bol2 << endl;
	
	cout << "Durata: " << Somma_Durate(bol2) << endl;

	return 0;
}
