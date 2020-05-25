// references:
// http://lazyfoo.net/tutorials/SDL/11_clip_rendering_and_sprite_sheets/index.php

#include <SDL2/SDL.h>
#include <cstring>

#include "wyngine/game.h"
#include "wyngine/keyboard.h"
#include "wyngine/image.h"
#include "wyngine/random.h"
#include "wyngine/font.h"

class Game : public Wyngine
{
private:
    SDL_Event windowEvent;
    SDL_Texture *tFont;
    WY_MonoFont *wyFont;
    int pixelSize = 4;

    bool loadMedia()
    {
        bool success = true;

        tFont = loadPNG(windowRenderer, "assets/ascii-bnw.png");
        if (tFont == NULL)
        {
            printf("Failed to load image!\n");
            success = false;
        }

        return success;
    }

    bool createFont()
    {
        wyFont = new WY_MonoFont(tFont, 8, 4, pixelSize, {10, 10});

        return true;
    }

public:
    Game() : Wyngine("Wyngine", 640, 480, pixelSize)
    {
        gameRunning = gameRunning && loadMedia();
        gameRunning = gameRunning && createFont();
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

    void onDraw()
    {
        // TODO draw text
    }
};

int main(int argc, char *args[])
{
    Game *game = new Game();

    game->run();

    return 0;
}