#OBJS specifies which files to compile as part of the project
OBJS = main01.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = bin\main01

#This is the target that compiles our executable
all : $(OBJS)
	g++ -g $(OBJS) \
	-IC:\wy-dev\sdl2-mingw-32\include\SDL2 \
	-LC:\wy-dev\sdl2-mingw-32\lib \
	-lmingw32 -lSDL2main -lSDL2 \
	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
	-w -Wl,-subsystem,windows \
	-static \
	-o $(OBJ_NAME)

# web build (-03 flag helps!)
# emcc main01.cpp -O3 -s USE_SDL=2 -o bin-js/webgame.html