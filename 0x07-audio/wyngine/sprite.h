#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class WY_Sprite
{
    bool mActive = false;
    SDL_Texture *mTexture = nullptr;
    SDL_Rect mRectSource{0, 0, 0, 0};
    SDL_Rect mRectTarget{0, 0, 0, 0};

public:
    bool getActive()
    {
        return mActive;
    }

    WY_Sprite() {}

    WY_Sprite(SDL_Texture *t, SDL_Rect rs, SDL_Rect rt)
    {
        init(t, rs, rt);
    }

    ~WY_Sprite()
    {
        mTexture = nullptr;
        delete &mRectSource;
        delete &mRectTarget;
    }

    void init(SDL_Texture *t, SDL_Rect rs, SDL_Rect rt)
    {
        mTexture = t;
        mRectSource = rs;
        mRectTarget = rt;
    }

    void render(SDL_Renderer *renderer, int gameW, int gameH)
    {
        // don't render if outside of screen.
        if (mRectTarget.x + mRectTarget.w < 0 ||
            mRectTarget.y + mRectTarget.h < 0 ||
            mRectTarget.x > gameW ||
            mRectTarget.y > gameH)
        {
            return;
        }

        SDL_RenderCopy(renderer, mTexture, &mRectSource, &mRectTarget);
    }

    void activate()
    {
        mActive = true;
    }

    void kill()
    {
        mActive = false;
    }
};
