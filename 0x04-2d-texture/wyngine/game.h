#include <SDL2/SDL.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void emscriptenOnUpdate(void *arg);

class Wyngine
{
protected:
    const char *windowTitle;
    int screenW;
    int screenH;
    int pixelSize;
    int texW;
    int texH;
    bool gameRunning;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

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

        if (init())
        {
            gameRunning = true;
        }
    }

    ~Wyngine()
    {
        SDL_DestroyWindow(window);
        window = NULL;

        SDL_Quit();
    }

    virtual void onUpdate() {}

    void run()
    {
#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(emscriptenOnUpdate, this, 0, 1);
#else
        while (gameRunning)
        {
            onUpdate();
        }
#endif
    }

private:
    bool init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenW, screenH, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texW, texH);

        return true;
    }
};

// Reference: https://stackoverflow.com/questions/55415179/unable-to-pass-a-proper-lambda-to-emscripten-set-main-loop
// Emscripten only supports plain functions, not member functions
void emscriptenOnUpdate(void *arg)
{
    static_cast<Wyngine *>(arg)->onUpdate();
}