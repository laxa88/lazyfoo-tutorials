// references:
// http://lazyfoo.net/tutorials/SDL/11_clip_rendering_and_sprite_sheets/index.php

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
    SDL_Texture *tSprite = NULL;

    // TODO
    // add animation logic for spritesheet
    // publish new post

    bool loadMedia()
    {
        bool success = true;

        tSprite = loadPNG(windowRenderer, "assets/sprites.png");
        if (tSprite == NULL)
        {
            printf("Failed to load image!\n");
            success = false;
        }

        return success;
    }

    void loadSprites()
    {
        addSprite(new WY_Sprite(tSprite, {0, 0, 50, 50}, {0, 0, 50, 50}), true);
        addSprite(new WY_Sprite(tSprite, {50, 0, 50, 50}, {getScreenW() - 50, 0, 50, 50}), true);
        addSprite(new WY_Sprite(tSprite, {0, 50, 50, 50}, {0, getScreenH() - 50, 50, 50}), true);
        addSprite(new WY_Sprite(tSprite, {50, 50, 50, 50}, {getScreenW() - 50, getScreenH() - 50, 50, 50}), true);
    }

public:
    Game() : Wyngine("Wyngine", 640, 480, 4)
    {
        if (gameRunning)
        {
            loadMedia();
            loadSprites();
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
                }
            }
        }
    }
};

int main(int argc, char *args[])
{
    Game *game = new Game();

    game->run();

    return 0;
}