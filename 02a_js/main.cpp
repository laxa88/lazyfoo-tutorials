#include <SDL2/SDL.h>
#include <string>
#include <stdio.h>
#include <emscripten.h>
#include <emscripten/html5.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();

SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;
SDL_Surface *imageSurface = NULL;
SDL_Event windowEvent;
int frameCount = 0;

bool init()
{
    bool success = true;
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            screenSurface = SDL_GetWindowSurface(window);
        }
    }

    return success;
}

bool loadMedia()
{
    bool success = true;

    imageSurface = SDL_LoadBMP("img/hello_world.bmp");

    if (imageSurface == NULL)
    {
        printf("Unable to load image %s! SDL Error: %s\n", "img/hello_world.bmp", SDL_GetError());
        success = false;
    }

    return success;
}

void close()
{
    SDL_FreeSurface(screenSurface);
    screenSurface = NULL;

    SDL_FreeSurface(imageSurface);
    imageSurface = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

// EM_BOOL one_iter(double time, void *userData)
// {
//     // Can render to the screen here, etc.
//     puts("one iteration");

//     SDL_BlitSurface(imageSurface, NULL, screenSurface, NULL);

//     SDL_UpdateWindowSurface(window);

//     // Return true to keep the loop running.
//     return EM_TRUE;
// }

void js_game_loop()
{
    ++frameCount;

    std::string foo1 = std::to_string(frameCount);
    std::string foo2 = "frame " + foo1;

    puts(foo2.c_str());

    SDL_BlitSurface(imageSurface, NULL, screenSurface, NULL);

    SDL_UpdateWindowSurface(window);
}

void game_loop()
{
#ifdef __EMSCRIPTEN__
    // does not blit surface, probably meant for physics
    // emscripten_request_animation_frame_loop(one_iter, 0);

    // render
    emscripten_set_main_loop(js_game_loop, 30, 1);
#else
    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                break;
            }
        }

        SDL_BlitSurface(imageSurface, NULL, screenSurface, NULL);

        SDL_UpdateWindowSurface(window);
    }
#endif
}

int main(int argc, char *args[])
{
    if (!init())
    {
        printf("Failed to initialize!\n");
        return 0;
    }

    if (!loadMedia())
    {
        printf("Failed to load media!\n");
        return 0;
    }

    game_loop();

    close();

    return 0;
}