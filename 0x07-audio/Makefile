# Note: Don't optimize with -O when debugging; It screws breakpoints
# all : audio2.cpp
# 	g++ -g audio2.cpp \
# 	-IC:\wy-dev\sdl2-mingw-32\include \
# 	-IC:\wy-dev\sdl2-mingw-32\include\SDL2 \
# 	-IC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\include \
# 	-LC:\wy-dev\sdl2-mingw-32\lib \
# 	-LC:\wy-dev\sdl2-mingw-32\lib\SDL2 \
# 	-LC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\lib \
# 	-lmingw32 -lSDL2main -lSDL2 -lSDL2_image  \
# 	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
# 	-static \
# 	-o bin\audio2

all : audio.c
	gcc -g audio.c \
	-IC:\wy-dev\sdl2-mingw-32\include \
	-IC:\wy-dev\sdl2-mingw-32\include\SDL2 \
	-IC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\include \
	-LC:\wy-dev\sdl2-mingw-32\lib \
	-LC:\wy-dev\sdl2-mingw-32\lib\SDL2 \
	-LC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\lib \
	-lmingw32 -lSDL2main -lSDL2 -lSDL2_image  \
	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
	-static \
	-o bin\audio

# all : main2.cpp
# 	g++ -g main2.cpp \
# 	-IC:\wy-dev\sdl2-mingw-32\include \
# 	-IC:\wy-dev\sdl2-mingw-32\include\SDL2 \
# 	-IC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\include \
# 	-LC:\wy-dev\sdl2-mingw-32\lib \
# 	-LC:\wy-dev\sdl2-mingw-32\lib\SDL2 \
# 	-LC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\lib \
# 	-lmingw32 -lSDL2main -lSDL2 -lSDL2_image  \
# 	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
# 	-static \
# 	-o bin\main2

# Sample C++ command
# all : main.cpp
# 	g++ main.cpp -ggdb3 -O3 \
# 	-Wall -Wextra -pedantic \
# 	-IC:\wy-dev\sdl2-mingw-32\include \
# 	-LC:\wy-dev\sdl2-mingw-32\lib \
# 	-lmingw32 -lSDL2main -lSDL2  \
# 	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
# 	-static \
# 	-o bin\main

# all : main.c
# 	gcc main.c -ggdb3 -O3 -std=c99 \
# 	-Wall -Wextra -pedantic \
# 	-IC:\wy-dev\sdl2-mingw-32\include \
# 	-LC:\wy-dev\sdl2-mingw-32\lib \
# 	-lmingw32 -lSDL2main -lSDL2 \
# 	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
# 	-static \
# 	-o bin\main

# all : wy-test.cpp
# 	g++ -g wy-test.cpp \
# 	-IC:\wy-dev\sdl2-mingw-32\include \
# 	-IC:\wy-dev\sdl2-mingw-32\include\SDL2 \
# 	-IC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\include \
# 	-LC:\wy-dev\sdl2-mingw-32\lib \
# 	-LC:\wy-dev\sdl2-mingw-32\lib\SDL2 \
# 	-LC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\lib \
# 	-lmingw32 -lSDL2main -lSDL2 -lSDL2_image  \
# 	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
# 	-static \
# 	-o bin\wy-test

# web build (-03 flag helps!)
# emcc main.c -O3 -s USE_SDL=2 -o bin-js/webgame.html