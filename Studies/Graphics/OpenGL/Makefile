CXX=c++
CXXFLAG=-Wall -O2

dummy_build_folder := $(shell mkdir -p build)

main: src/main.cpp src/GPU/glad/glad.h glad src/GPU/Shader.h shader src/GPU/stb_image/stb_image.h stb_image
	$(CXX) $(CXFLAG) src/main.cpp build/glad.o build/Shader.o build/stb_image.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -o build/main

glad : src/GPU/glad/glad.c
	$(CXX) $(CXFLAG) -c src/GPU/glad/glad.c -o build/glad.o

shader : src/GPU/glad/glad.h glad
	$(CXX) $(CXFLAG) -c src/GPU/Shader.cpp -o build/Shader.o

stb_image: src/GPU/stb_image/stb_image.h
	$(CXX) $(CXFLAG) -c src/GPU/stb_image/stb_image.cpp -o build/stb_image.o

clean :
	rm -rf build
