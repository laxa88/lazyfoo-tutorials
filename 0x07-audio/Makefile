# Note: Don't optimize with -O when debugging; It screws breakpoints
# run `mingw32-make` to invoke "all" rule
# run `mingw32-make <rule-name>` to invoke specific rule

all : \
	audio2-cpp \
	audio-c \
	main2-cpp \
	main-cpp \
	main-c \
	wy-test-cpp

audio2-cpp:
	g++ -g audio2.cpp \
	-IC:\wy-dev\sdl2-mingw-32\include \
	-IC:\wy-dev\sdl2-mingw-32\include\SDL2 \
	-IC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\include \
	-LC:\wy-dev\sdl2-mingw-32\lib \
	-LC:\wy-dev\sdl2-mingw-32\lib\SDL2 \
	-LC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\lib \
	-lmingw32 -lSDL2main -lSDL2 -lSDL2_image  \
	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
	-static \
	-o bin\audio2

audio-c:
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

main2-cpp:
	g++ -g main2.cpp \
	-IC:\wy-dev\sdl2-mingw-32\include \
	-IC:\wy-dev\sdl2-mingw-32\include\SDL2 \
	-IC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\include \
	-LC:\wy-dev\sdl2-mingw-32\lib \
	-LC:\wy-dev\sdl2-mingw-32\lib\SDL2 \
	-LC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\lib \
	-lmingw32 -lSDL2main -lSDL2 -lSDL2_image  \
	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
	-static \
	-o bin\main2

main-cpp:
	g++ main.cpp -ggdb3 -O3 \
	-Wall -Wextra -pedantic \
	-IC:\wy-dev\sdl2-mingw-32\include \
	-LC:\wy-dev\sdl2-mingw-32\lib \
	-lmingw32 -lSDL2main -lSDL2  \
	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
	-static \
	-o bin\main-cpp

main-c:
	gcc main.c -ggdb3 -O3 -std=c99 \
	-Wall -Wextra -pedantic \
	-IC:\wy-dev\sdl2-mingw-32\include \
	-LC:\wy-dev\sdl2-mingw-32\lib \
	-lmingw32 -lSDL2main -lSDL2 \
	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
	-static \
	-o bin\main-c

wy-test-cpp:
	g++ -g wy-test.cpp \
	-IC:\wy-dev\sdl2-mingw-32\include \
	-IC:\wy-dev\sdl2-mingw-32\include\SDL2 \
	-IC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\include \
	-LC:\wy-dev\sdl2-mingw-32\lib \
	-LC:\wy-dev\sdl2-mingw-32\lib\SDL2 \
	-LC:\wy-dev\SDL2_image-2.0.5\i686-w64-mingw32\lib \
	-lmingw32 -lSDL2main -lSDL2 -lSDL2_image  \
	-lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lsetupapi -lversion \
	-static \
	-o bin\wy-test

# web build (-03 flag helps!)
# emcc main.c -O3 -s USE_SDL=2 -o bin-js/webgame.html
# emcc main2.cpp -O3 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] --preload-file assets -o bin-js/webgame.html
