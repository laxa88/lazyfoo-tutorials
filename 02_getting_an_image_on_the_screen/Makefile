#OBJS specifies which files to compile as part of the project
OBJS = main.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

# original full command
# g++ $(OBJS) -IC:\wy-dev\sdl2-mingw-32\include\SDL2 -LC:\wy-dev\sdl2-mingw-32\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -o $(OBJ_NAME)

#This is the target that compiles our executable
all : $(OBJS)
	g++ $(OBJS) -IC:\wy-dev\sdl2-mingw-32\include\SDL2 -LC:\wy-dev\sdl2-mingw-32\lib -lmingw32 -lSDL2main -lSDL2 -o $(OBJ_NAME)