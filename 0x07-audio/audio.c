// ref: Hands-on Game Development with Web Assembly by Rick Battagline

#include <SDL2/SDL.h>
#include <emscripten.h>
#include <stdio.h>
#include <stdbool.h>

#define LASER "/assets/laser.wav"
#define EXPLOSION "/assets/explosion.wav"

SDL_AudioDeviceID device_id;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

struct audio_clip
{
    char file_name[100];
    SDL_AudioSpec spec;
    Uint32 len;
    Uint8 *buf;
} laser_snd, explosion_snd;

void play_audio(struct audio_clip *clip)
{
    int success = SDL_QueueAudio(device_id, clip->buf, clip->len);
    if (success < 0)
    {
        printf("SDL_QueueAudio %s failed: %s\n", clip->file_name, SDL_GetError());
    }
}

void init_audio(char *file_name, struct audio_clip *clip)
{
    strcpy(clip->file_name, file_name);

    if (SDL_LoadWAV(file_name, &(clip->spec), &(clip->buf), &(clip->len)) == NULL)
    {
        printf("Failed to load wav file: %s\n", SDL_GetError());
    }
}

void input_loop()
{
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_1:
                printf("key 1 release\n");
                play_audio(&laser_snd);
                break;

            case SDLK_2:
                printf("key 2 release\n");
                play_audio(&explosion_snd);
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1))
    {
        printf("Could not init SDL: %s\n", SDL_GetError());
        return 0;
    }

    SDL_CreateWindowAndRenderer(320, 200, 0, &window, &renderer);

    init_audio(LASER, &laser_snd);
    init_audio(EXPLOSION, &explosion_snd);

    device_id = SDL_OpenAudioDevice(NULL, 0, &(laser_snd.spec), NULL, 0);

    if (device_id == 0)
    {
        printf("Failed to open audio: %s\n", SDL_GetError());
    }

    SDL_PauseAudioDevice(device_id, 0);

    emscripten_set_main_loop(input_loop, 0, 1);

    return 1;
}

// emcc audio.c --preload-file assets -s USE_SDL=2 -o audio.html