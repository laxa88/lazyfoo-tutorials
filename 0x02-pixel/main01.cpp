#include <SDL.h>
#include <stdio.h>
#include <random>
#include <cstring>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

    return true;
}

void close()
{
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
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

    int posX = 0, posY = 0;
    int texW = SCREEN_WIDTH / 10;
    int texH = SCREEN_HEIGHT / 10;
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texW, texH);

    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                break;
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
                unsigned char rgba[4] = {rand() % 255, rand() % 255, rand() % 255, rand() % 255};
                memcpy(&bytes[(y * texW + x) * sizeof(rgba)], rgba, sizeof(rgba));
            }
        }
        SDL_UnlockTexture(texture);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    close();

    return 0;
}