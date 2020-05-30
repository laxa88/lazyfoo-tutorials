#OBJS specifies which files to compile as part of the project
OBJS = main.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = bin\main

#This is the target that compiles our executable
all : $(OBJS)
	g++ -g $(OBJS) \
	-IC:\wy-dev\sdl2-mingw-32\include \
	-IC:\wy-dev\sdl2-mingw-32\include\SDL2 \
	-IC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\include \
	-LC:\wy-dev\sdl2-mingw-32\lib \
	-LC:\wy-dev\sdl2-mingw-32\lib\SDL2 \
	-LC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\lib \
	-lmingw32 -lSDL2main -lSDL2 -lSDL2_image  \
	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
	-static \
	-o $(OBJ_NAME)

# Sample C code that works: https://stackoverflow.com/questions/10110905/simple-sound-wave-generator-with-sdl-in-c
# all : main.c
# 	gcc -ggdb3 -O3 -std=c99 \
# 	-Wall -Wextra -pedantic \
# 	-IC:\wy-dev\sdl2-mingw-32\include \
# 	-LC:\wy-dev\sdl2-mingw-32\lib \
# 	-lmingw32 -lSDL2main -lSDL2 \
# 	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
# 	-o main.exe main.c -lSDL2 -lm

# web build (-03 flag helps!)
# emcc main.cpp -O3 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] --preload-file assets -o bin-js/webgame.html