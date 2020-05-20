// https://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php

#include <SDL2/SDL.h>
#include <string>

SDL_Texture *loadTexture(SDL_Renderer *renderer, std::string path)
{
    SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());
    SDL_Texture *newTexture = NULL;

    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}
