// references:
// https://lazyfoo.net/tutorials/SDL/04_key_presses/index.php
// https://lazyfoo.net/tutorials/SDL/05_optimized_surface_loading_and_soft_stretching/index.php
// https://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php

#include <SDL2/SDL.h>
#include <cstring>

#include "wyngine/game.h"
#include "wyngine/keyboard.h"
#include "wyngine/image.h"
#include "wyngine/random.h"

class Game : public Wyngine
{
private:
    SDL_Event windowEvent;

    SDL_Texture *gKeyPressTextures[KEY_PRESS_SURFACE_TOTAL];
    SDL_Texture *gCurrentTexture = NULL;

    // TODO
    // replace PNG load with PNG load
    // check desktop build and size
    // check JS output and build size
    // refactor to use spritesheet instead
    // add animation logic for spritesheet
    // publish new post

    bool loadMedia()
    {
        //Loading success flag
        bool success = true;

        //Load default surface
        gKeyPressTextures[KEY_PRESS_SURFACE_DEFAULT] = loadPNG(renderer, "assets/press.bmp");
        if (gKeyPressTextures[KEY_PRESS_SURFACE_DEFAULT] == NULL)
        {
            printf("Failed to load default image!\n");
            success = false;
        }

        //Load up surface
        gKeyPressTextures[KEY_PRESS_SURFACE_UP] = loadPNG(renderer, "assets/up.bmp");
        if (gKeyPressTextures[KEY_PRESS_SURFACE_UP] == NULL)
        {
            printf("Failed to load up image!\n");
            success = false;
        }

        //Load down surface
        gKeyPressTextures[KEY_PRESS_SURFACE_DOWN] = loadPNG(renderer, "assets/down.bmp");
        if (gKeyPressTextures[KEY_PRESS_SURFACE_DOWN] == NULL)
        {
            printf("Failed to load down image!\n");
            success = false;
        }

        //Load left surface
        gKeyPressTextures[KEY_PRESS_SURFACE_LEFT] = loadPNG(renderer, "assets/left.bmp");
        if (gKeyPressTextures[KEY_PRESS_SURFACE_LEFT] == NULL)
        {
            printf("Failed to load left image!\n");
            success = false;
        }

        //Load right surface
        gKeyPressTextures[KEY_PRESS_SURFACE_RIGHT] = loadPNG(renderer, "assets/right.bmp");
        if (gKeyPressTextures[KEY_PRESS_SURFACE_RIGHT] == NULL)
        {
            printf("Failed to load right image!\n");
            success = false;
        }

        return success;
    }

public:
    void foo()
    {
    }

    Game() : Wyngine("Wyngine", 640, 480, 2)
    {
        if (gameRunning)
        {
            loadMedia();

            gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_DEFAULT];
        }
    }

    void onUpdate()
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                gameRunning = false;
            }
            else if (windowEvent.type == SDL_KEYDOWN)
            {
                switch (windowEvent.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    gameRunning = false;
                    break;

                case SDLK_UP:
                    gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_UP];
                    break;

                case SDLK_DOWN:
                    gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_DOWN];
                    break;

                case SDLK_LEFT:
                    gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_LEFT];
                    break;

                case SDLK_RIGHT:
                    gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_RIGHT];
                    break;

                default:
                    gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_DEFAULT];
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // Initialize texture pixels to a red opaque RGBA value
        // unsigned char *bytes = nullptr;
        // int pitch = 0; // row of pixels
        // SDL_LockTexture(texture, NULL, reinterpret_cast<void **>(&bytes), &pitch);
        // for (int y = 0; y < texH; y++)
        // {
        //     for (int x = 0; x < texW; x++)
        //     {
        //         unsigned char rgba[4] = {random(255), random(255), random(255), random(255)};
        //         memcpy(&bytes[(y * texW + x) * sizeof(rgba)], rgba, sizeof(rgba));
        //     }
        // }
        // SDL_UnlockTexture(texture);

        // SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderCopy(renderer, gCurrentTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
};

int main(int argc, char *args[])
{
    Game *game = new Game();

    game->run();

    return 0;
}