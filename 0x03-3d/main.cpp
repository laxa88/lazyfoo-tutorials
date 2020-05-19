// https://gist.github.com/jordandee/94b187bcc51df9528a2f

// To compile with gcc:  (tested on Ubuntu 14.04 64bit):
//	 g++ sdl2_opengl.cpp -lSDL2 -lGL
// To compile with msvc: (tested on Windows 7 64bit)
//   cl sdl2_opengl.cpp /I C:\sdl2path\include /link C:\path\SDL2.lib C:\path\SDL2main.lib /SUBSYSTEM:CONSOLE /NODEFAULTLIB:libcmtd.lib opengl32.lib

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <random>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

#define WinWidth 480
#define WinHeight 320

u32 WindowFlags = SDL_WINDOW_OPENGL;
SDL_Window *Window;

b32 Running = 1;
b32 FullScreen = 0;

void init()
{
    Window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WinWidth, WinHeight, WindowFlags);
    assert(Window);
    SDL_GLContext Context = SDL_GL_CreateContext(Window);
}

int random(int mod, float div)
{
#ifdef __EMSCRIPTEN__
    return emscripten_random() * mod / div;
#else
    return rand() % mod / div;
#endif
}

void game_loop()
{
    SDL_Event Event;
    while (SDL_PollEvent(&Event))
    {
        if (Event.type == SDL_KEYDOWN)
        {
            switch (Event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                Running = 0;
                break;
            case 'f':
                FullScreen = !FullScreen;
                if (FullScreen)
                {
                    SDL_SetWindowFullscreen(Window, WindowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
                else
                {
                    SDL_SetWindowFullscreen(Window, WindowFlags);
                }
                break;
            default:
                break;
            }
        }
        else if (Event.type == SDL_QUIT)
        {
            Running = 0;
        }
    }

    glViewport(0, 0, WinWidth, WinHeight);
    glClearColor(1.f, 0.f, 1.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // https://stackoverflow.com/questions/42405420/how-to-draw-a-single-pixel-in-opengl
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glPointSize(1);
    glOrtho(0.0, WinWidth, WinHeight, 0.0, 0.0, 100.0);
    glBegin(GL_POINTS);
    for (int y = 0; y < WinHeight; y++)
    {
        for (int x = 0; x < WinWidth; x++)
        {
            // not supported by webGL
            glColor3f(random(200, 100.0), random(200, 100.0), random(200, 100.0));
            glVertex2i(x, y);
        }
    }
    glEnd();

    SDL_GL_SwapWindow(Window);
}

void main_loop()
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(game_loop, 0, 1);
#else
    while (Running)
    {
        game_loop();
    }
#endif
}

int main(int ArgCount, char **Args)
{
    init();

    main_loop();

    return 0;
}