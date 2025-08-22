
build/game.exe: build/main.o build/glad.o build/camera.o | build
	- g++ build/*.o -o build/game.exe -L lib -lglfw3dll -lopengl32

build/main.o: src/main.cpp src/*.hpp | build
	- g++ -std=c++14 -O3 -c src/main.cpp -o build/main.o -I include -I src
build/glad.o: src/glad.c | build
	- g++ -std=c++14 -O3 -c src/glad.c -o build/glad.o -I include
build/camera.o: src/camera.cpp src/camera.hpp | build
	- g++ -std=c++14 -O3 -c src/camera.cpp -o build/camera.o -I include

build:
	-@ powershell -Command mkdir build
	-@ powershell -Command cp lib/glfw3.dll build

.PHONY = run
run: build/game.exe
	-@ ./build/game.exe

.PHONY = clean
clean:
	-@ powershell -Command Remove-Item build -Recurse