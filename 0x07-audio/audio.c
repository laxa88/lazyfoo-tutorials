// ref: Hands-on Game Development with Web Assembly by Rick Battagline

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// PLAYBACK logic

#define LASER "assets/laser.wav"
#define EXPLOSION "assets/explosion.wav"

SDL_AudioDeviceID device_id;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;
bool gameRunning = true;

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
        printf("SDL_QueueAudio %s failed: %s\n", clip->file_name, SDL_GetError());
}

void init_audio(char *file_name, struct audio_clip *clip)
{
    strcpy(clip->file_name, file_name);

    if (SDL_LoadWAV(file_name, &(clip->spec), &(clip->buf), &(clip->len)) == NULL)
    {
        printf("Failed to load wav file: %s\n", SDL_GetError());
    }
}

void init_playback()
{
    if (device_id != 0)
        SDL_CloseAudioDevice(device_id);

    device_id = SDL_OpenAudioDevice(NULL, 0, &(laser_snd.spec), NULL, 0);

    if (device_id == 0)
        printf("Failed to open audio: %s\n", SDL_GetError());

    SDL_PauseAudioDevice(device_id, 0);
}

// SYNTH logic

const int AMPLITUDE = 28000;
const int SAMPLE_RATE = 44100;
Uint32 floatStreamLength = 1024;
SDL_atomic_t audioCallbackLeftOff;
Uint32 audioBufferLength = 48000;
float *audioBuffer;
SDL_AudioSpec want;
SDL_AudioSpec have;

// void audio_callback(void *unused, Uint8 *byteStream, int byteStreamLength)
// {
//     float *floatStream = (float *)byteStream;
//     Sint32 localAudioCallbackLeftOff = SDL_AtomicGet(&audioCallbackLeftOff);
//     Uint32 i;
//     for (i = 0; i < floatStreamLength; i++)
//     {
//         floatStream[i] = audioBuffer[localAudioCallbackLeftOff];
//         localAudioCallbackLeftOff++;
//         if (localAudioCallbackLeftOff == audioBufferLength)
//             localAudioCallbackLeftOff = 0;
//     }
//     SDL_AtomicSet(&audioCallbackLeftOff, localAudioCallbackLeftOff);
// }

void audio_callback(void *user_data, Uint8 *raw_buffer, int bytes)
{
    Sint16 *buffer = (Sint16 *)raw_buffer;
    int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS
    // int &sample_nr(*(int *)user_data); // valid c but crashes
    // int &sample_nr = (*(int *)user_data); // valid c++ but crashes
    // int sample_nr(*(int *)user_data); // c, stutters but works
    int sample_nr = (*(int *)user_data); // c++, stutters but works
    // printf("\ncallback1: %d, %d, %d", *(int *)user_data, &user_data, user_data);

    int *sample_nr2 = (int *)user_data;
    printf("\ncurr %d, %d", (int *)user_data, (*(int *)user_data));
    // printf("\ncurr %d, %d", sample_nr2, *sample_nr2);
    // printf("\nwant %d, %d", want.userdata, &(want.userdata));
    // printf("\nhave %d, %d", have.userdata, &(want.userdata));

    for (int i = 0; i < length; i++, sample_nr++)
    {
        double time = (double)sample_nr / (double)SAMPLE_RATE;
        buffer[i] = (Sint16)(AMPLITUDE * sin(2.0f * M_PI * 441.0f * time)); // render 441 HZ sine wave
    }
}

void init_synth()
{
    int sample_nr = 0;

    printf("\ninit %d, %d", &sample_nr, sample_nr);

    want.freq = SAMPLE_RATE;        // number of samples per second
    want.format = AUDIO_S16SYS;     // sample type (here: signed short i.e. 16 bit)
    want.channels = 1;              // only one channel
    want.samples = 2048;            // buffer-size
    want.callback = audio_callback; // function SDL calls periodically to refill the buffer
    want.userdata = &sample_nr;     // counter, keeping track of current sample number

    if (device_id != 0)
        SDL_CloseAudioDevice(device_id);

    device_id = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

    if (device_id == 0)
        printf("\nFailed to open audio: %s", SDL_GetError());
    else
        printf("\nOpened audio ID: %d", device_id);

    if (want.format != have.format)
        printf("\nFailed to get the desired AudioSpec");

    SDL_PauseAudioDevice(device_id, 0);
}

void start_synth()
{
    SDL_PauseAudioDevice(device_id, 0);
}

void pause_synth()
{
    SDL_PauseAudioDevice(device_id, 1);
}

// MAIN logic

void input_loop()
{
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                gameRunning = false;
                break;

            case SDLK_1:
                printf("key 1 release\n");
                play_audio(&laser_snd);
                break;

            case SDLK_2:
                printf("key 2 release\n");
                play_audio(&explosion_snd);
                break;

            case SDLK_3:
                printf("key 3 release\n");
                start_synth();
                break;

            case SDLK_4:
                printf("key 4 release\n");
                pause_synth();
                break;

            case SDLK_5:
                printf("key 5 release\n");
                init_synth();
                break;

            case SDLK_6:
                printf("key 6 release\n");
                init_playback();
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

    init_playback();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(input_loop, 0, 1);
#else
    while (gameRunning)
    {
        input_loop();
    }
#endif

    return 1;
}

// 2020-05-31 - works for desktop and web, for audio playback and synth, but stutters
// emcc audio.c --preload-file assets -s USE_SDL=2 -o bin-js/audio.html