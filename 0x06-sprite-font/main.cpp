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

    void loadMedia()
    {
        bool success = true;

        tFont = loadPNG(mRenderer, "assets/ascii-bnw.png");
        if (tFont == NULL)
        {
            printf("Failed to load image!\n");
            success = false;
        }
    }

    void createFont()
    {
        wyFont = new WY_MonoFont(tFont, 8, 4, {10, 10});
    }

public:
    Game(const char *title, int w, int h, int ps) : Wyngine(title, w, h, ps)
    {
        if (mGameRunning)
        {
            loadMedia();
            createFont();
        }
    }

    void onUpdate()
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                mGameRunning = false;
            }
            else if (windowEvent.type == SDL_KEYDOWN)
            {
                switch (windowEvent.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    mGameRunning = false;
                    break;
                }
            }
        }
    }

    void onRender()
    {
        wyFont->print(mRenderer, 1, 1, "Hello world!\nNew line!");
    }
};

int main(int argc, char *args[])
{
    Game *game = new Game("0x06 font sprite", 320, 240, 2);

    game->run();

    return 0;
}