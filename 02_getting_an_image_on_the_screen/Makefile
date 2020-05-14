#OBJS specifies which files to compile as part of the project
OBJS = main.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

# original full command
# g++ $(OBJS) -IC:\wy-dev\sdl2-mingw-32\include\SDL2 -LC:\wy-dev\sdl2-mingw-32\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -o $(OBJ_NAME)

#This is the target that compiles our executable

# statically compiles, results in ~7.7mb single executable
# all : $(OBJS)
# 	g++ $(OBJS) \
# 	-IC:\wy-dev\sdl2-mingw-32\include\SDL2 \
# 	-LC:\wy-dev\sdl2-mingw-32\lib \
# 	-w -Wl,-subsystem,windows \
# 	-Wl,--trace \
# 	-lmingw32 -lSDL2main -lSDL2 \
# 	-lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -static-libgcc \
# 	-static \
# 	-o $(OBJ_NAME)

# add this to trace which files are used during dynamic linkage
# -Wl,--trace

# dynamically compiles, results in 64kb executable, but requires .dll files
all : $(OBJS)
	g++ $(OBJS) \
	-IC:\wy-dev\sdl2-mingw-32\include\SDL2 \
	-LC:\wy-dev\sdl2-mingw-32\lib \
	-w -Wl,-subsystem,windows \
	-Wl,--trace \
	-lmingw32 -lSDL2main -lSDL2 \
	-o $(OBJ_NAME)