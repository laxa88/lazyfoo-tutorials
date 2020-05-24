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
        bool success = true;

        //Load default surface
        gKeyPressTextures[KEY_PRESS_SURFACE_DEFAULT] = loadPNG(windowRenderer, "assets/press.png");
        if (gKeyPressTextures[KEY_PRESS_SURFACE_DEFAULT] == NULL)
        {
            printf("Failed to load default image!\n");
            success = false;
        }

        //Load up surface
        gKeyPressTextures[KEY_PRESS_SURFACE_UP] = loadPNG(windowRenderer, "assets/up.png");
        if (gKeyPressTextures[KEY_PRESS_SURFACE_UP] == NULL)
        {
            printf("Failed to load up image!\n");
            success = false;
        }

        //Load down surface
        gKeyPressTextures[KEY_PRESS_SURFACE_DOWN] = loadPNG(windowRenderer, "assets/down.png");
        if (gKeyPressTextures[KEY_PRESS_SURFACE_DOWN] == NULL)
        {
            printf("Failed to load down image!\n");
            success = false;
        }

        //Load left surface
        gKeyPressTextures[KEY_PRESS_SURFACE_LEFT] = loadPNG(windowRenderer, "assets/left.png");
        if (gKeyPressTextures[KEY_PRESS_SURFACE_LEFT] == NULL)
        {
            printf("Failed to load left image!\n");
            success = false;
        }

        //Load right surface
        gKeyPressTextures[KEY_PRESS_SURFACE_RIGHT] = loadPNG(windowRenderer, "assets/right.png");
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

    Game() : Wyngine("Wyngine", 640, 480, 4)
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
    }

    void onDraw()
    {
        SDL_RenderCopy(windowRenderer, gCurrentTexture, NULL, NULL);
    }
};

int main(int argc, char *args[])
{
    Game *game = new Game();

    game->run();

    return 0;
}