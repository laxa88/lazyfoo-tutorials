// https://lazyfoo.net/tutorials/SDL/41_bitmap_fonts/index.php

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class WY_MonoFont
{
    SDL_Texture *texture = nullptr;
    SDL_Rect bound{0, 0, 0, 0};
    SDL_Rect rDest{0, 0, 0, 0};
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

    int getAscii(std::string text, int index)
    {
        // mini-ascii offsets by first 32 unused glyphs
        return (unsigned char)text[index] - 32;
    }

public:
    int getFontSize()
    {
        return fontSize * pixelSize;
    }

    int getVPadding()
    {
        return vPadding * pixelSize;
    }

    int getBoundW()
    {
        return bound.w * pixelSize;
    }

    int getBoundH()
    {
        return bound.h * pixelSize;
    }

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

    // void printd(SDL_Renderer *renderer, int x, int y, std::string text)
    // {
    //     int rs = getFontSize();

    //     for (int c = 0; c < text.length(); c++)
    //     {
    //         int ascii = (unsigned char)text[c];
    //         printf("\nprint: %c (%d, %d)", text[c], c, ascii);
    //         rDest = {(c * rs) % 320, (c * rs) / 320 * rs, rs, rs};
    //         SDL_RenderCopy(renderer, texture, &rChars[ascii - 32], &rDest);
    //     }
    // }

    void print(SDL_Renderer *renderer, int x, int y, std::string text)
    {
        if (texture == nullptr)
        {
            printf("Font texture missing!\n");
            return;
        }

        /*
            get word length (until next space char)
                if first word in line
                    print word regardless
                else if word does not exceed bound
                    print word
                else (word exceeds bound)
                    move to next line
        */

        currX = x * pixelSize;
        currY = y * pixelSize;
        int fs = getFontSize();
        int cPos = 1;
        int row = 0;

        for (int c = 0; c < text.length(); c++)
        {
            int ascii = getAscii(text, c);
            int cPosAscii = getAscii(text, cPos);

            while (cPosAscii != ' ' && cPosAscii != '\n' && cPos < text.length())
            {
                cPos++;
                cPosAscii = getAscii(text, cPos);
            }

            // TODO setup debugger!
            // TODO this doesn't look right; doesn't consider x-origin
            int currWidth = cPos * fs;

            // if bound width is zero, it means there's no bound
            // if first word OR width is less than bound width, draw!
            if (getBoundW() == 0 || currX % getBoundW() == 0 || currWidth <= getBoundW())
            {
                // draw word
                while (c < cPos)
                {
                    currX = (x * pixelSize) + (c * fs);

                    ascii = getAscii(text, c);
                    rDest = {currX, currY, fs, fs};
                    SDL_RenderCopy(renderer, texture, &rChars[ascii], &rDest);

                    c++;
                }
            }

            // if width is more than bound width OR encountered newline, force to new line
            if (getBoundW() > 0 && (currWidth > getBoundW() || ascii == '\n'))
            {
                row++;
                currX = x * pixelSize;
                currY = ((y * pixelSize) + (getBoundH() + getVPadding())) * row;
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
