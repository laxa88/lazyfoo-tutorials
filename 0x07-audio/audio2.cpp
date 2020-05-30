// https://stackoverflow.com/questions/10110905/simple-sound-wave-generator-with-sdl-in-c

#include <math.h>
#include <stdio.h>
#include <SDL2/SDL.h>

const int AMPLITUDE = 28000;
const int SAMPLE_RATE = 44100;

void audio_callback(void *user_data, Uint8 *raw_buffer, int bytes)
{
    Sint16 *buffer = (Sint16 *)raw_buffer;
    int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS
    int &sample_nr(*(int *)user_data);

    for (int i = 0; i < length; i++, sample_nr++)
    {
        double time = (double)sample_nr / (double)SAMPLE_RATE;
        buffer[i] = (Sint16)(AMPLITUDE * sin(2.0f * M_PI * 441.0f * time)); // render 441 HZ sine wave
    }
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_AUDIO) != 0)
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());

    int sample_nr = 0;

    SDL_AudioSpec want;
    want.freq = SAMPLE_RATE;        // number of samples per second
    want.format = AUDIO_S16SYS;     // sample type (here: signed short i.e. 16 bit)
    want.channels = 1;              // only one channel
    want.samples = 2048;            // buffer-size
    want.callback = audio_callback; // function SDL calls periodically to refill the buffer
    want.userdata = &sample_nr;     // counter, keeping track of current sample number

    SDL_AudioSpec have;
    if (SDL_OpenAudio(&want, &have) != 0)
        printf("Failed to open audio: %s", SDL_GetError());
    if (want.format != have.format)
        printf("Failed to get the desired AudioSpec");

    SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

    if (device_id == 0)
        printf("Failed to open audio: %s\n", SDL_GetError());

    SDL_PauseAudioDevice(device_id, 0); // start playing sound
    SDL_Delay(3000);                    // wait while sound is playing
    SDL_PauseAudioDevice(device_id, 1); // stop playing sound

    SDL_CloseAudio();

    return 0;
}