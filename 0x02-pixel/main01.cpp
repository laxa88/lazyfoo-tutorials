#include <SDL.h>

int main(int argc, char *argv[])
{
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);

    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Log("Worked fine!");
    SDL_Quit();

    return 0;
}