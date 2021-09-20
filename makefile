.PHONY=run

game.exe: main.cpp glad.c
	-@ g++ -std=c++11 main.cpp -o game.exe -I ./include -L ./lib -lglfw3dll -lopengl32
hello.exe: hello.cpp
	-@ g++ -std=c++11 hello.cpp -o hello.exe -I ./include -L ./lib -lglfw3dll -lopengl32
run: game.exe
	-@ ./game.exe