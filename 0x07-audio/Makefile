# output
OBJ_NAME = bin\main

# Sample C++ command
all : main.cpp
	g++ main.cpp -g -ggdb3 -O3 \
	-Wall -Wextra -pedantic \
	-IC:\wy-dev\sdl2-mingw-32\include \
	-LC:\wy-dev\sdl2-mingw-32\lib \
	-lmingw32 -lSDL2main -lSDL2  \
	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
	-static \
	-o $(OBJ_NAME)

# Sample C command that works: https://stackoverflow.com/questions/10110905/simple-sound-wave-generator-with-sdl-in-c
# all : main.c
# 	gcc main.c -g -ggdb3 -O3 -std=c99 \
# 	-Wall -Wextra -pedantic \
# 	-IC:\wy-dev\sdl2-mingw-32\include \
# 	-LC:\wy-dev\sdl2-mingw-32\lib \
# 	-lmingw32 -lSDL2main -lSDL2 \
# 	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
# 	-static \
# 	-o $(OBJ_NAME)

# web build (-03 flag helps!)
# emcc main.c -O3 -s USE_SDL=2 -o bin-js/webgame.html