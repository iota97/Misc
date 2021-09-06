CXX=c++
CXXFLAG=-Wall -O2

dummy_build_folder := $(shell mkdir -p build)

main: src/main.cpp src/glad/glad.h glad
	$(CXX) $(CXFLAG) src/main.cpp build/glad.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -o build/main

glad : src/glad/glad.c
	$(CXX) $(CXFLAG) -c src/glad/glad.c -o build/glad.o
	
clean :
	rm -rf build
