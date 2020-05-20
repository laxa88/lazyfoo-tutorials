// based on https://lazyfoo.net/tutorials/SDL/04_key_presses/index.php

#include <SDL2/SDL.h>
#include <stdio.h>
#include <cstring>

#include "game.h"
#include "keyboard.h"
#include "image.h"

SDL_Event windowEvent;

SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Texture *gKeyPressTextures[KEY_PRESS_SURFACE_TOTAL];

// bool loadMedia()
// {
//     //Loading success flag
//     bool success = true;

//     //Load default surface
//     gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadTexture("04_key_presses/press.bmp");
//     if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
//     {
//         printf("Failed to load default image!\n");
//         success = false;
//     }

//     //Load up surface
//     gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadTexture("04_key_presses/up.bmp");
//     if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
//     {
//         printf("Failed to load up image!\n");
//         success = false;
//     }

//     //Load down surface
//     gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadTexture("04_key_presses/down.bmp");
//     if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
//     {
//         printf("Failed to load down image!\n");
//         success = false;
//     }

//     //Load left surface
//     gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadTexture("04_key_presses/left.bmp");
//     if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
//     {
//         printf("Failed to load left image!\n");
//         success = false;
//     }

//     //Load right surface
//     gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadTexture("04_key_presses/right.bmp");
//     if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
//     {
//         printf("Failed to load right image!\n");
//         success = false;
//     }

//     return success;
// }

class Game : public Wyngine
{
public:
    Game() : Wyngine("Wyngine", 640, 480, 2)
    {
    }

    void onUpdate()
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                gameRunning = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // Initialize texture pixels to a red opaque RGBA value
        unsigned char *bytes = nullptr;
        int pitch = 0; // row of pixels
        SDL_LockTexture(texture, NULL, reinterpret_cast<void **>(&bytes), &pitch);
        // draw
        SDL_UnlockTexture(texture);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
};

int main(int argc, char *args[])
{
    Game *game = new Game();

    game->run();

    return 0;
}