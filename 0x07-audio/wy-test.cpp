// 2020-05-31: works for desktop and web
// emcc wy-test.cpp -s USE_SDL=2 -o bin-js/audio.html

#include <cstdint>
#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <cstring>

#include "wyngine/game.h"
#include "wyngine/keyboard.h"
#include "wyngine/image.h"
#include "wyngine/random.h"

class Sound
{
public:
    Sound();
    ~Sound();
    void play();
    void stop();

    const double m_sineFreq;
    const double m_sampleFreq;
    const double m_samplesPerSine;
    uint32_t m_samplePos;

private:
    static void SDLAudioCallback(void *data, Uint8 *buffer, int length);

    SDL_AudioDeviceID m_device;
};

Sound::Sound()
    : m_sineFreq(1000),
      m_sampleFreq(44100),
      m_samplesPerSine(m_sampleFreq / m_sineFreq),
      m_samplePos(0)
{
    printf("\ninit start");

    SDL_AudioSpec wantSpec, haveSpec;

    SDL_zero(wantSpec);
    wantSpec.freq = m_sampleFreq;
    wantSpec.format = AUDIO_U8;
    wantSpec.channels = 1;
    wantSpec.samples = 2048;
    wantSpec.callback = SDLAudioCallback;
    wantSpec.userdata = this;

    m_device = SDL_OpenAudioDevice(NULL, 0, &wantSpec, &haveSpec, 0);
    if (m_device == 0)
    {
        std::cout << "Failed to open audio: " << SDL_GetError() << std::endl;
    }

    printf("\ninit done: %d", m_device);
}

Sound::~Sound()
{
    SDL_CloseAudioDevice(m_device);
}

void Sound::play()
{
    printf("\nstart play");
    SDL_PauseAudioDevice(m_device, 0);
}

void Sound::stop()
{
    printf("\nstop play");
    SDL_PauseAudioDevice(m_device, 1);
}

void Sound::SDLAudioCallback(void *data, Uint8 *buffer, int length)
{
    printf("\naudio playback");
    Sound *sound = reinterpret_cast<Sound *>(data);

    for (int i = 0; i < length; ++i)
    {
        buffer[i] = (std::sin(sound->m_samplePos / sound->m_samplesPerSine * M_PI * 2) + 1) * 127.5;
        ++sound->m_samplePos;
    }
}

class Game : public Wyngine
{
    SDL_Event windowEvent;
    SDL_Texture *tSprite = NULL;
    Sound *sound;

public:
    Game()
    {
        SDL_Init(SDL_INIT_AUDIO);

        sound = new Sound();

        sound->play();
    }

    void onUpdate()
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                mGameRunning = false;
            }
            else if (windowEvent.type == SDL_KEYDOWN)
            {
                switch (windowEvent.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    mGameRunning = false;
                    break;

                case SDLK_1:
                    sound->play();
                    break;

                case SDLK_2:
                    sound->stop();
                    break;
                }
            }
        }
    }
};

int main(int argc, char *args[])
{
    Game *game = new Game();

    game->run();

    return 0;
}