#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Sprite
{
    int frame = 0;
    SDL_Texture *texture = nullptr;
    SDL_Rect rectSource{0, 0, 0, 0};
    SDL_Rect rectTarget{0, 0, 0, 0};

public:
    Sprite::Sprite()
    {
    }

    Sprite::Sprite(SDL_Texture *t, int x, int y, int w, int h)
    {
        texture = t;
        rectSource = {x, y, w, h};
    }

    Sprite::~Sprite()
    {
        texture = nullptr;
        delete &rectSource;
        delete &rectTarget;
    }

    void render(SDL_Renderer *renderer, float pixelSize)
    {
        SDL_RenderCopy(renderer, texture, &rectSource, &rectTarget);
    }
};
