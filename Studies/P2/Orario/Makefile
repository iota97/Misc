CXX=c++
CXXFLAG=-Wall -O2

p2.out: src/main.cpp build/orario.o build/telefonata.o build/bolletta.o
	$(CXX) $(CXFLAG) src/main.cpp build/orario.o build/telefonata.o build/bolletta.o -o p2.out

build/orario.o : src/orario.cpp src/orario.h
	$(CXX) $(CXFLAG) -c src/orario.cpp -o build/orario.o

build/telefonata.o : src/telefonata.cpp src/telefonata.h
	$(CXX) $(CXFLAG) -c src/telefonata.cpp -o build/telefonata.o

build/bolletta.o : src/bolletta.cpp src/bolletta.h
	$(CXX) $(CXFLAG) -c src/bolletta.cpp -o build/bolletta.o
	
clean :
	rm build/*.o
