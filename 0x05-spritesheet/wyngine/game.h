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

        window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowW, windowH, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        windowRenderer = SDL_CreateRenderer(window, -1, 0);
        SDL_SetRenderDrawColor(windowRenderer, 0xCC, 0xCC, 0xCC, 0xCC);

        screenTexture = SDL_CreateTexture(windowRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texW, texH);

        return true;
    }

protected:
    const char *windowTitle;
    int windowW;
    int windowH;
    int pixelSize;
    int texW;
    int texH;
    bool gameRunning;

    SDL_Window *window = NULL;
    SDL_Renderer *windowRenderer = NULL;
    SDL_Texture *screenTexture = NULL; // unused; reserve for camera using SDL_RENDERER_TARGETTEXTURE
    std::vector<WY_Sprite *> spritePool;

    virtual void onUpdate() {}

    virtual void onDraw() {}

public:
    int getScreenW()
    {
        return windowW / pixelSize;
    }

    int getScreenH()
    {
        return windowH / pixelSize;
    }

    Wyngine() : Wyngine("Wyngine", 640, 480, 1)
    {
    }

    Wyngine(const char *title, int w, int h, int ps)
    {
        windowTitle = title;
        windowW = w;
        windowH = h;
        pixelSize = ps;
        texW = w / ps;
        texH = h / ps;

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
        SDL_Rect r{rand() % 500, rand() % 500, 200, 200};

        // perform internal draw here
        SDL_RenderClear(windowRenderer);

        int len = spritePool.size();
        for (int i = 0; i < len; i++)
        {
            WY_Sprite *sprite = spritePool.at(i);
            sprite->draw(windowRenderer, pixelSize, windowW, windowH);
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

    void addSprite(WY_Sprite *sprite, bool activate)
    {
        spritePool.push_back(sprite);
        sprite->activate();
    }

    void addSprite(WY_Sprite *sprite)
    {
        addSprite(sprite, false);
    }

    // Returns an inactive sprite. If non available, will add to pool before returning.
    WY_Sprite *getAvailableSprite()
    {
        for (int i = 0; i < spritePool.size(); i++)
        {
            if (!spritePool.at(i)->getActive())
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