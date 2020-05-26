// http://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index2.php
// https://lazyfoo.net/tutorials/SDL/43_render_to_texture/index.php

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

struct WY_Image
{
    SDL_Texture *texture;
    int w, h;
};

WY_Image *loadPNG(SDL_Renderer *renderer, std::string path)
{
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    SDL_Texture *newTexture = NULL;
    WY_Image *newImage = new WY_Image();

    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0);
        if (formattedSurface == NULL)
        {
            printf("Unable to convert loaded surface to display format! %s\n", SDL_GetError());
        }
        else
        {
            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

            newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
            if (newTexture == NULL)
            {
                printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
            }
            else
            {
                void *mPixels = NULL;
                int mPitch = 0;
                int w, h;

                SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);

                SDL_LockTexture(newTexture, &formattedSurface->clip_rect, &mPixels, &mPitch);

                memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);
                newImage->w = formattedSurface->w;
                newImage->h = formattedSurface->h;

                // Color key pixels
                Uint32 *pixels = (Uint32 *)mPixels;
                int pixelCount = (mPitch / 4) * newImage->h;
                Uint32 colorKey = SDL_MapRGB(formattedSurface->format, 0xFF, 0xFF, 0xFF);
                Uint32 transparent = SDL_MapRGBA(formattedSurface->format, 0x00, 0xFF, 0xFF, 0x00);
                for (int i = 0; i < pixelCount; ++i)
                {
                    if (pixels[i] == colorKey)
                    {
                        pixels[i] = transparent;
                    }
                }

                SDL_UnlockTexture(newTexture);
            }

            SDL_FreeSurface(formattedSurface);
        }

        SDL_FreeSurface(loadedSurface);
    }

    newImage->texture = newTexture;
    return newImage;
}
