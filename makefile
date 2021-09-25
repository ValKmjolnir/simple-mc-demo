.PHONY=run

game.exe: main.cpp glad.c block.h camera.h noise.h
	-@ g++ -std=c++11 -O3 main.cpp -o game.exe -I ./include -L ./lib -lglfw3dll -lopengl32
run: game.exe
	-@ ./game.exe