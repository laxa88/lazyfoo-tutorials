#include <SDL.h>
#include <stdio.h>
#include <random>
#include <cstring>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int texW = SCREEN_WIDTH / 10;
const int texH = SCREEN_HEIGHT / 10;
bool game_running = true;

bool init();
bool loadMedia();
void close();

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Event windowEvent;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texW, texH);

    return true;
}

void close()
{
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

unsigned char random(int mod)
{
#ifdef __EMSCRIPTEN__
    return emscripten_random() * mod;
#else
    return rand() % mod;
#endif
}

void game_loop()
{
    if (SDL_PollEvent(&windowEvent))
    {
        if (SDL_QUIT == windowEvent.type)
        {
            game_running = false;
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Initialize texture pixels to a red opaque RGBA value
    unsigned char *bytes = nullptr;
    int pitch = 0; // row of pixels
    SDL_LockTexture(texture, NULL, reinterpret_cast<void **>(&bytes), &pitch);
    for (int y = 0; y < texH; y++)
    {
        for (int x = 0; x < texW; x++)
        {
            unsigned char rgba[4] = {random(255), random(255), random(255), random(255)};
            memcpy(&bytes[(y * texW + x) * sizeof(rgba)], rgba, sizeof(rgba));
        }
    }
    SDL_UnlockTexture(texture);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void main_loop()
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(game_loop, 30, 1);
#else
    while (game_running)
    {
        game_loop();
    }
#endif
}

int main(int argc, char *args[])
{
    //Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
        close();
        return 0;
    }

    main_loop();

    close();

    return 0;
}