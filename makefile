.PHONY=run

game.exe: src/main.cpp src/glad.c src/block.h src/camera.h src/noise.h
# 	-@ g++ -std=c++14 -O3 src/main.cpp -o game.exe -I include -I src -L lib -lglfw3dll -lopengl32
	-@ g++ -std=c++14 -O3 src/main.cpp -o game.exe -I include -I src -L lib -lglfw3dll -lopengl32
run: game.exe
	-@ ./game.exe