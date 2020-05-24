#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class WY_Sprite
{
    bool active = false;
    int frame = 0;
    SDL_Texture *texture = nullptr;
    SDL_Rect rectSource{0, 0, 0, 0};
    SDL_Rect rectTarget{0, 0, 0, 0};
    SDL_Rect rectTargetScaled{0, 0, 0, 0};

public:
    bool getActive()
    {
        return active;
    }

    WY_Sprite()
    {
    }

    WY_Sprite(SDL_Texture *t, SDL_Rect rs, SDL_Rect rt)
    {
        init(t, rs, rt);
    }

    ~WY_Sprite()
    {
        texture = nullptr;
        delete &rectSource;
        delete &rectTarget;
    }

    void init(SDL_Texture *t, SDL_Rect rs, SDL_Rect rt)
    {
        texture = t;

        // delete &rSource;
        rectSource = rs;

        // delete &rTarget;
        rectTarget = rt;
    }

    void draw(SDL_Renderer *renderer, int pixelSize, int screenW, int screenH)
    {
        rectTargetScaled.x = rectTarget.x * pixelSize;
        rectTargetScaled.y = rectTarget.y * pixelSize;
        rectTargetScaled.w = rectTarget.w * pixelSize;
        rectTargetScaled.h = rectTarget.h * pixelSize;

        // printf("\nSource %d %d %d %d", rectSource.x, rectSource.y, rectSource.w, rectSource.w);
        // printf("\nTarget %d %d %d %d", rectTargetScaled.x, rectTargetScaled.y, rectTargetScaled.w, rectTargetScaled.w);

        // don't render if outside of screen.
        // TODO handle rotated/scaled sprites
        if (rectTargetScaled.x + rectTargetScaled.w < 0 ||
            rectTargetScaled.y + rectTargetScaled.h < 0 ||
            rectTargetScaled.x > screenW ||
            rectTargetScaled.y > screenH)
        {
            return;
        }

        SDL_RenderCopy(renderer, texture, &rectSource, &rectTargetScaled);
    }

    void activate()
    {
        active = true;
    }

    void kill()
    {
        active = false;
    }
};
