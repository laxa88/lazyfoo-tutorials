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
    WY_Image *mFontImage;
    WY_MonoFont *mFont;

    void loadMedia()
    {
        bool success = true;

        mFontImage = loadPNG(mRenderer, "assets/ascii-bnw.png");
        if (mFontImage == NULL)
        {
            printf("Failed to load image!\n");
            success = false;
        }
    }

    void createFont()
    {
        mFont = new WY_MonoFont(mFontImage->texture, 8, 4, {10, 10, 120, 50});
        mFont->setDebug(true);
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
        mFont->print(mRenderer, "Hello wrapped world!\n\nNew line!");
    }
};

int main(int argc, char *args[])
{
    Game *game = new Game("0x06 font sprite", 256, 224, 3);

    game->run();

    return 0;
}