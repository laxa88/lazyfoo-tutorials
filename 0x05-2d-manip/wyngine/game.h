#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iterator>
#include <vector>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "sprite.h"

void emscriptenLoop(void *arg);

class Wyngine
{
private:
    bool init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
            return false;
        }

        window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenW, screenH, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        windowRenderer = SDL_CreateRenderer(window, -1, 0);
        screenTexture = SDL_CreateTexture(windowRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texW, texH);

        return true;
    }

protected:
    const char *windowTitle;
    int screenW;
    int screenH;
    int pixelSize;
    int texW;
    int texH;
    bool gameRunning;

    SDL_Window *window = NULL;
    SDL_Renderer *windowRenderer = NULL;
    SDL_Texture *screenTexture = NULL;
    std::vector<WY_Sprite *> spritePool;

    virtual void onUpdate() {}

    virtual void onDraw() {}

public:
    Wyngine() : Wyngine("Wyngine", 640, 480, 2)
    {
    }

    Wyngine(const char *title, int w, int h, int ps)
    {
        windowTitle = title;
        screenW = w;
        screenH = h;
        pixelSize = ps;
        texW = w / ps;
        texH = h / ps;

        for (int i = 0; i < 1000; i++)
        {
            spritePool.push_back(new WY_Sprite());
        }

        if (init())
        {
            gameRunning = true;
        }
    }

    ~Wyngine()
    {
        SDL_DestroyWindow(window);
        window = NULL;

        for (int i = 0; i < spritePool.size(); i++)
        {
            delete spritePool.at(i);
        }
        spritePool.clear();

        SDL_Quit();
    }

    void update()
    {
        // perform internal physics here

        onUpdate();
    }

    void draw()
    {
        // perform internal draw here
        SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 0);
        SDL_RenderClear(windowRenderer);

        // SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderCopy(windowRenderer, screenTexture, NULL, NULL);

        // Initialize texture pixels to a red opaque RGBA value
        // unsigned char *bytes = nullptr;
        // int pitch = 0; // row of pixels
        // SDL_LockTexture(texture, NULL, reinterpret_cast<void **>(&bytes), &pitch);
        // for (int y = 0; y < texH; y++)
        // {
        //     for (int x = 0; x < texW; x++)
        //     {
        //         unsigned char rgba[4] = {random(255), random(255), random(255), random(255)};
        //         memcpy(&bytes[(y * texW + x) * sizeof(rgba)], rgba, sizeof(rgba));
        //     }
        // }
        // SDL_UnlockTexture(texture);
        int len = spritePool.size();
        for (int i = 0; i < len; i++)
        {
            WY_Sprite *sprite = spritePool.at(i);
            if (sprite->alive())
            {
                sprite->draw(windowRenderer, pixelSize);
            }
        }

        onDraw();

        SDL_RenderPresent(windowRenderer);
    }

    void gameLoop()
    {
        update();
        draw();
    }

    // Entry point
    void run()
    {
#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(emscriptenLoop, this, 0, 1);
#else
        while (gameRunning)
        {
            gameLoop();
        }
#endif
    }

    // Returns an inactive sprite. If non available, will add to pool before returning.
    WY_Sprite *getSprite(WY_Sprite *sprite)
    {
        for (int i = 0; i < spritePool.size(); i++)
        {
            if (!spritePool.at(i)->alive())
            {
                return spritePool.at(i);
            }
        }

        WY_Sprite *newSprite = new WY_Sprite();
        spritePool.push_back(newSprite);
        return newSprite;
    }
};

// Reference: https://stackoverflow.com/questions/55415179/unable-to-pass-a-proper-lambda-to-emscripten-set-main-loop
// Emscripten only supports plain functions, not member functions
void emscriptenLoop(void *arg)
{
    static_cast<Wyngine *>(arg)->gameLoop();
}