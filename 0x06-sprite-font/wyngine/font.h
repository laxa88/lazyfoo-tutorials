// https://lazyfoo.net/tutorials/SDL/41_bitmap_fonts/index.php

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class WY_MonoFont
{
    SDL_Texture *texture = nullptr;
    SDL_Rect bound{0, 0, 0, 0};
    SDL_Rect rChars[256];
    int fontSize;  // monospace fonts assumed to be equal w/h
    int vPadding;  // vertical padding between lines
    int pixelSize; // screen resolution
    int currX, currY;
    int texW, texH;

    void buildFonts()
    {
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

        // Assumes font spritesheet uses ascii layout
        for (int i = 0; i < 256; i++)
        {
            rChars[i] = {(i * fontSize) % texW, (i * fontSize / texW) * fontSize, fontSize, fontSize};
        }
    }

public:
    WY_MonoFont(SDL_Texture *t, int fs, int vp, int ps, SDL_Rect b)
    {
        texture = t;
        fontSize = fs;
        vPadding = vp;
        pixelSize = ps;

        if (&bound != nullptr)
        {
            delete &bound;
        }

        bound = b;

        buildFonts();
    }

    WY_MonoFont(SDL_Texture *t, int fs, int vp, int ps) : WY_MonoFont(t, fs, vp, ps, {0, 0, 0, 0}) {}
    WY_MonoFont(SDL_Texture *t, int fs, int vp) : WY_MonoFont(t, fs, vp, 1, {0, 0, 0, 0}) {}
    WY_MonoFont(SDL_Texture *t, int fs) : WY_MonoFont(t, fs, 4, 1, {0, 0, 0, 0}) {}
    WY_MonoFont(SDL_Texture *t) : WY_MonoFont(t, 8, 4, 1, {0, 0, 0, 0}) {}

    ~WY_MonoFont()
    {
        texture = nullptr;
        delete &bound;
    }

    void print(SDL_Renderer *renderer, int x, int y, std::string text)
    {
        if (texture == nullptr)
        {
            printf("Font texture missing!\n");
            return;
        }

        currX = bound.x;
        currY = bound.y;

        /*
            get word length (until next space char)
                if first word in line
                    print word regardless
                else if word does not exceed bound
                    print word
                else (word exceeds bound)
                    move to next line
        */

        int cPos = 1;
        for (int c = 0, c < text.length(); c++)
        {
            while (text[cPos] != ' ' && text[cPos] != '\n' && cPos < text.length())
            {
                cPos++;
            }

            currX = c * fontSize;
            int currWidth = cPos * fontSize;

            // if first word OR width is less than bound width, draw!
            if (currX % bound.w == 0 || currWidth <= bound.w)
            {
                // draw word
                while (c < cPos)
                {
                    SDL_RenderCopy(renderer, texture, rChars[c], {currX, currY, fontSize * pixelSize, fontSize * pixelSize});
                    c++;
                }
            }

            // if width is more than bound width OR encountered newline, force to new line
            if (currWidth > bound.w || text[cPos] == '\n')
            {
                currX = 0;
                currY += (bound.h + vPadding);
            }

            cPos++;
        }

        // IMPROVEMENTS
        // - colorize font
        // - animate to appear letter by letter
        // - animate each character with wobbly effect
        // - add custom animation per character
    }
};
