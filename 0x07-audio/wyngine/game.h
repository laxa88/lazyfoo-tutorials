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
protected:
    const char *windowTitle;
    int mGameW;  // actual game width
    int mGameH;  // actual game height
    int mGamePS; // pixel size
    bool mGameRunning;

    SDL_Window *mWindow = NULL;
    SDL_Renderer *mRenderer = NULL;
    SDL_Texture *mTexture = NULL;
    std::vector<WY_Sprite *> spritePool;

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

        mWindow = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mGameW * mGamePS, mGameH * mGamePS, SDL_WINDOW_SHOWN);
        if (mWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        // https://stackoverflow.com/questions/18647592/sdl-render-texture-on-top-of-another-texture
        mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_TARGETTEXTURE);

        SDL_SetRenderDrawColor(mRenderer, 0x00, 0xFF, 0x00, 0xFF);

        mTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mGameW, mGameH);

        return true;
    }

    virtual void onUpdate() {}

    virtual void onRender() {}

public:
    Wyngine(const char *title, int w, int h, int ps)
    {
        windowTitle = title;
        mGameW = w;
        mGameH = h;
        mGamePS = ps;

        if (init())
        {
            mGameRunning = true;
        }
    }

    Wyngine() : Wyngine("Wyngine", 640, 480, 1) {}

    ~Wyngine()
    {
        SDL_DestroyWindow(mWindow);
        mWindow = NULL;

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

    void render()
    {
        // perform internal render here

        SDL_SetRenderTarget(mRenderer, mTexture);

        // Clear with magenta so we know this works
        SDL_SetRenderDrawColor(mRenderer, 0xEE, 0xEE, 0xEE, 0xFF);
        SDL_RenderClear(mRenderer);

        int len = spritePool.size();
        for (int i = 0; i < len; i++)
        {
            WY_Sprite *sprite = spritePool.at(i);
            sprite->render(mRenderer, mGameW, mGameH);
        }

        onRender();

        // Unset mTexture as render target before rendering mTexture to mRenderer
        SDL_SetRenderTarget(mRenderer, NULL);

        // Renders mTexture to fit to window size; Smaller textures will result in retro feel
        SDL_RenderCopy(mRenderer, mTexture, NULL, NULL);

        // Flush to screen
        SDL_RenderPresent(mRenderer);
    }

    void gameLoop()
    {
        update();
        render();
    }

    // Entry point
    void run()
    {
#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(emscriptenLoop, this, 0, 1);
#else
        while (mGameRunning)
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