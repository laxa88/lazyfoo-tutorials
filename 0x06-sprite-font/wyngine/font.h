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
    bool mDebug;

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

    int getFullAscii(std::string text, int index)
    {
        return (unsigned char)text[index];
    }

    int getMiniAscii(std::string text, int index)
    {
        // mini-ascii offsets by first 32 unused glyphs
        return (unsigned char)text[index] - 32;
    }

public:
    void setDebug(bool flag)
    {
        mDebug = flag;
    }

    WY_MonoFont(SDL_Texture *t, int fs, int vp, SDL_Rect b)
    {
        mTexture = t;
        mFontSize = fs;
        mPaddingV = vp;
        mBound = b;
        mDebug = false;

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

    void print(SDL_Renderer *renderer, std::string text)
    {
        if (mTexture == nullptr)
        {
            printf("Font texture missing!\n");
            return;
        }

        if (mDebug)
        {
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); // green
            SDL_RenderDrawRect(renderer, &mBound);
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

        mCurrX = mBound.x;
        mCurrY = mBound.y;

        for (int c = 0; c < text.length(); c++)
        {
            int fullAscii = getFullAscii(text, c);
            while (fullAscii == '\n')
            {
                c++;
                mCurrY += (mPaddingV + mFontSize);
                mCurrX = mBound.x;

                fullAscii = getFullAscii(text, c);
            }

            // int fullAscii = getFullAscii(text, c);
            // printf("\n#%d : %c = %d, %d", c, fullAscii, mCurrX, mCurrY);

            int miniAscii = getMiniAscii(text, c);
            mDest = {mCurrX, mCurrY, mFontSize, mFontSize};
            SDL_RenderCopy(renderer, mTexture, &mChars[miniAscii], &mDest);

            mCurrX += mFontSize;
        }

        // IMPROVEMENTS
        // - colorize font
        // - animate to appear letter by letter
        // - animate each character with wobbly effect
        // - add custom animation per character
    }
};
