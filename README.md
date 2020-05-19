### Requisites

- Download [SDL2](https://www.libsdl.org/download-2.0.php). In this repo's case, I use MinGW, so download the MinGW SDL dev library, e.g. `SDL2-devel-2.0.12-mingw.tar.gz`

### Build

#### Desktop

- Install [MinGW](https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe/)
- Update `Makefile`'s Include paths for SDL `include` and `lib` folder.
- Run `mingw32-make.exe` within the folder that contains the `Makefile`.

#### OSX

- Use brew: `brew update` > `brew upgrade`
- Install GCC: `brew install gcc`
- Install SDL2 and take advantage of brew's default directory: `brew install sdl2`
- Run `g++ main.cpp -o main -L/usr/local/lib -lSDL2`

Notes:

- The result is a unix executable, which opens terminal to run the game.
- TODO: Figure out how to bundle the game into `.app` executable for distribution.

#### JS

- Install emscripten
- Open `emcmdprompt.bat` (which contains `emcc` command) from installation folder
- Run `emcc` with built-in SDL libs, e.g. `emcc main.cpp -O3 -s USE_SDL=2 -s USE_SDL_IMAGE=2 --preload-file img -o js/webgame.html`

### Debug

- Remove `-Wl,-subsystem,windows` from the `Makefile` to allow `std::cout` to be output into the console when the `.exe` is run.
- Alternatively, output stream into a file, e.g. run `main.exe > log.txt`

### Notes

- [GCC docs for options](https://gcc.gnu.org/onlinedocs/gcc-3.4.6/gcc/Invoking-GCC.html#Invoking-GCC)

### additional reading

https://lyceum-allotments.github.io/2016/06/emscripten-and-sdl2-tutorial-part-4-look-owl/
