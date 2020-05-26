// https://lazyfoo.net/tutorials/SDL/41_bitmap_fonts/index.php

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class WY_MonoFont
{
    SDL_Texture *mTexture = nullptr;
    SDL_Rect mBound{0, 0, 0, 0};
    SDL_Rect mDest{0, 0, 0, 0};
    SDL_Rect mChars[256];
    int mFontSize; // monospace fonts assumed to be equal w/h
    int mPaddingV; // vertical padding between lines
    int mCurrX, mCurrY;

    void buildFonts()
    {
        int texW, texH;
        SDL_QueryTexture(mTexture, NULL, NULL, &texW, &texH);

        // Assumes font spritesheet uses ascii layout
        for (int i = 0; i < 256; i++)
        {
            mChars[i] = {
                (i * mFontSize) % texW,
                (i * mFontSize / texW) * mFontSize,
                mFontSize,
                mFontSize};
        }
    }

    int getAscii(std::string text, int index)
    {
        // mini-ascii offsets by first 32 unused glyphs
        return (unsigned char)text[index] - 32;
    }

public:
    // int getFontSize()
    // {
    //     return fontSize * pixelSize;
    // }

    // int getVPadding()
    // {
    //     return vPadding * pixelSize;
    // }

    // int getBoundW()
    // {
    //     return bound.w * pixelSize;
    // }

    // int getBoundH()
    // {
    //     return bound.h * pixelSize;
    // }

    WY_MonoFont(SDL_Texture *t, int fs, int vp, SDL_Rect b)
    {
        mTexture = t;
        mFontSize = fs;
        mPaddingV = vp;
        mBound = b;

        buildFonts();
    }

    WY_MonoFont(SDL_Texture *t, int fs, int vp) : WY_MonoFont(t, fs, vp, {0, 0, 0, 0}) {}
    WY_MonoFont(SDL_Texture *t, int fs) : WY_MonoFont(t, fs, 4, {0, 0, 0, 0}) {}
    WY_MonoFont(SDL_Texture *t) : WY_MonoFont(t, 8, 4, {0, 0, 0, 0}) {}

    ~WY_MonoFont()
    {
        mTexture = nullptr;
        delete &mBound;
        delete &mDest;
        delete &mChars;
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
        if (mTexture == nullptr)
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

        mCurrX = x;
        mCurrY = y;
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

            // TODO this doesn't look right; doesn't consider x-origin
            int currWidth = cPos * mFontSize;

            // if bound width is zero, it means there's no bound
            // if first word OR width is less than bound width, draw!
            if (mBound.w == 0 || mCurrX % mBound.w == 0 || currWidth <= mBound.w)
            {
                // draw word
                while (c < cPos)
                {
                    mCurrX = x + (c * mFontSize);

                    ascii = getAscii(text, c);
                    mDest = {mCurrX, mCurrY, mFontSize, mFontSize};
                    SDL_RenderCopy(renderer, mTexture, &mChars[ascii], &mDest);

                    c++;
                }
            }

            // if width is more than bound width OR encountered newline, force to new line
            if (mBound.w > 0 && (currWidth > mBound.w || ascii == '\n'))
            {
                row++;
                mCurrX = x;
                mCurrY = (y + (mBound.h + mPaddingV)) * row;
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
