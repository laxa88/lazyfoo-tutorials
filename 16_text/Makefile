INPUT = main.cpp
OUTPUT = bin/main

# NOTE:
# - EXE requires dlls in the same folder to run.
# - run `main.exe > log.txt` to log errors

#This is the target that compiles our executable
all : $(INPUT)
	g++ $(INPUT) \
	-IC:\wy-dev\sdl2-mingw-32\include\SDL2 \
	-IC:\wy-dev\SDL2_ttf-2.0.15\i686-w64-mingw32\include\SDL2 \
	-IC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\include\SDL2 \
	-LC:\wy-dev\sdl2-mingw-32\lib \
	-LC:\wy-dev\SDL2_ttf-2.0.15\i686-w64-mingw32\lib \
	-LC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\lib \
	-w -Wl,-subsystem,windows \
	-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf \
	-o $(OUTPUT)