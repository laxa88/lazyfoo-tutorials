// https://stackoverflow.com/questions/10110905/simple-sound-wave-generator-with-sdl-in-c
// emcc audio2.cpp -O3 -s USE_SDL=2 -o bin-js/webgame.html

// 2020-05-31
// - desktop builds for sdl1, sdl2, web
// - sdl1 doesn't have sound
// - sdl2 has sound
// - web build doesn't have sound

#include <math.h>
#include <stdio.h>
#include <SDL2/SDL.h>

const int AMPLITUDE = 28000;
const int SAMPLE_RATE = 44100;
SDL_AudioSpec want;
SDL_AudioSpec have;

void audio_callback(void *user_data, Uint8 *raw_buffer, int bytes)
{
    Sint16 *buffer = (Sint16 *)raw_buffer;
    int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS
    int &sample_nr(*(int *)user_data);

    printf("callback: %d %d\n", sample_nr, have.freq);

    for (int i = 0; i < length; i++, sample_nr++)
    {
        double time = (double)sample_nr / (double)have.freq;
        buffer[i] = (Sint16)(AMPLITUDE * sin(2.0f * M_PI * 441.0f * time)); // render 441 HZ sine wave
    }
}

void play_sdl1()
{
    // for emscripten
    printf("Playing SDL1 audio\n");

    int sample_nr = 0;

    want.freq = SAMPLE_RATE;        // number of samples per second
    want.format = AUDIO_S16SYS;     // sample type (here: signed short i.e. 16 bit)
    want.channels = 1;              // only one channel
    want.samples = 2048;            // buffer-size
    want.callback = audio_callback; // function SDL calls periodically to refill the buffer
    want.userdata = &sample_nr;     // counter, keeping track of current sample number

    if (SDL_OpenAudio(&want, &have) != 0)
        printf("Failed to open audio 1: %s\n", SDL_GetError());

    if (want.format != have.format)
        printf("Failed to get the desired AudioSpec\n");

    printf("\n#want:\n");
    printf("freq: %d\n", want.freq);
    printf("format: %d\n", want.format);
    printf("channels: %d\n", want.channels);
    printf("samples: %d\n", want.samples);
    printf("userdata: %d\n", want.userdata);

    printf("\n#have:\n");
    printf("freq: %d\n", have.freq);
    printf("format: %d\n", have.format);
    printf("channels: %d\n", have.channels);
    printf("samples: %d\n", have.samples);
    printf("userdata: %d\n", have.userdata);

    printf("Playing sound...");
    SDL_PauseAudio(0); // start playing sound
    SDL_Delay(3000);   // wait while sound is playing
    SDL_PauseAudio(1); // stop playing sound
    SDL_CloseAudio();
    printf("Done.");
}

void play_sdl2()
{
    // for desktop
    printf("Playing SDL2 audio\n");

    int sample_nr = 0;

    // AUDIO_F32 = 33056
    // AUDIO_S16SYS = 32784

    want.freq = SAMPLE_RATE;        // number of samples per second
    want.format = AUDIO_S16SYS;     // sample type (here: signed short i.e. 16 bit)
    want.channels = 1;              // only one channel
    want.samples = 2048;            // buffer-size
    want.callback = audio_callback; // function SDL calls periodically to refill the buffer
    want.userdata = &sample_nr;     // counter, keeping track of current sample number

    SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

    if (want.format != have.format)
        printf("Failed to get the desired AudioSpec\n");

    printf("\ndevice_id: %d", device_id);

    printf("\n#want:\n");
    printf("freq: %d\n", want.freq);
    printf("format: %d\n", want.format);
    printf("channels: %d\n", want.channels);
    printf("samples: %d\n", want.samples);
    printf("userdata: %d\n", want.userdata);

    printf("\n#have:\n");
    printf("freq: %d\n", have.freq);
    printf("format: %d\n", have.format);
    printf("channels: %d\n", have.channels);
    printf("samples: %d\n", have.samples);
    printf("userdata: %d\n", have.userdata);

    if (device_id == 0)
        printf("Failed to open audio 2: %s\n", SDL_GetError());

    printf("Playing sound...");
    SDL_PauseAudioDevice(device_id, 0); // start playing sound
    SDL_Delay(3000);                    // wait while sound is playing
    SDL_PauseAudioDevice(device_id, 1); // stop playing sound
    SDL_CloseAudio();
    printf("Done.");
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_AUDIO) != 0)
        SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());

    // play_sdl1();
    play_sdl2();

    return 0;
}