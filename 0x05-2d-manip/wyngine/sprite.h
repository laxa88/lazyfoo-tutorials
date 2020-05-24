#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class WY_Sprite
{
    bool active = false;
    int frame = 0;
    SDL_Texture *texture = nullptr;
    SDL_Rect rectSource{0, 0, 0, 0};
    SDL_Rect rectTarget{0, 0, 0, 0};

public:
    bool alive()
    {
        return active;
    }

    WY_Sprite()
    {
    }

    WY_Sprite(SDL_Texture *t, int x, int y, int w, int h)
    {
        texture = t;
        rectSource = {x, y, w, h};
    }

    ~WY_Sprite()
    {
        texture = nullptr;
        delete &rectSource;
        delete &rectTarget;
    }

    void draw(SDL_Renderer *renderer, int pixelSize)
    {
        SDL_Rect scaledTarget = {
            rectTarget.x * pixelSize,
            rectTarget.y * pixelSize,
            rectTarget.w * pixelSize,
            rectTarget.h * pixelSize};
        SDL_RenderCopy(renderer, texture, &rectSource, &scaledTarget);
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
