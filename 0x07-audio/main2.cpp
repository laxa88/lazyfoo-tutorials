#include <SDL2/SDL.h>
#include <cstring>

#include "wyngine/game.h"
#include "wyngine/keyboard.h"
#include "wyngine/image.h"
#include "wyngine/random.h"
#include "wyngine/font.h"
#include "wyngine/audio.h"

class Game : public Wyngine
{
private:
    SDL_Event windowEvent;
    WY_Image *mFontImage;
    WY_MonoFont *mFont;
    WY_Audio *mAudio;

    void loadMedia()
    {
        bool success = true;

        mFontImage = loadPNG(mRenderer, "assets/ascii-bnw.png");
        if (mFontImage == NULL)
        {
            printf("Failed to load image!\n");
            success = false;
        }
    }

    void createFont()
    {
        mFont = new WY_MonoFont(mFontImage->texture, 8, 4, {10, 10, 80, 50});
        mFont->setDebug(true);
    }

    void createAudio()
    {
        mAudio = new WY_Audio();
        mAudio->play();
    }

public:
    Game(const char *title, int w, int h, int ps) : Wyngine(title, w, h, ps)
    {
        if (mGameRunning)
        {
            loadMedia();
            createFont();
            createAudio();
        }
    }

    void onUpdate()
    {
        while (SDL_PollEvent(&windowEvent) != 0)
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

                case SDLK_RIGHT:
                    mFont->setW(mFont->getW() + 5);
                    break;

                case SDLK_LEFT:
                    mFont->setW(mFont->getW() - 5);
                    break;

                case SDLK_a:
                    mAudio->pause();
                    break;

                case SDLK_p:
                    mAudio->play();
                    break;
                }
            }
        }

        mAudio->loop();
    }

    void onRender()
    {
        mFont->print(mRenderer, "Hello wrapped world!\n\nNew line!");
    }
};

// class WY_Audio
// {
//     float syncCompensationFactor = 0.0016;
//     Sint32 mainAudioLead;
//     Uint32 i;

//     voice testVoiceA;
//     voice testVoiceB;
//     voice testVoiceC;
//     Uint16 C0waveformLength = getWaveformLength(0);
//     float *sineWave;

// public:
//     void setup()
//     {
//         testVoiceA.volume = 1;
//         testVoiceB.volume = 1;
//         testVoiceC.volume = 1;
//         testVoiceA.pan = 0.5;
//         testVoiceB.pan = 0;
//         testVoiceC.pan = 1;
//         testVoiceA.phase = 0;
//         testVoiceB.phase = 0;
//         testVoiceC.phase = 0;
//         testVoiceA.frequency = getFrequency(45);
//         testVoiceB.frequency = getFrequency(49);
//         testVoiceC.frequency = getFrequency(52);
//         Uint16 C0waveformLength = getWaveformLength(0);
//         testVoiceA.waveformLength = C0waveformLength;
//         testVoiceB.waveformLength = C0waveformLength;
//         testVoiceC.waveformLength = C0waveformLength;
//         // float sineWave[C0waveformLength];
//         sineWave = new float[C0waveformLength]();
//         buildSineWave(sineWave, C0waveformLength);
//         testVoiceA.waveform = sineWave;
//         testVoiceB.waveform = sineWave;
//         testVoiceC.waveform = sineWave;
//     }

//     void play()
//     {
//         SDL_Delay(42);
//         SDL_PauseAudioDevice(AudioDevice, 0);
//         while (running)
//         {
//             while (SDL_PollEvent(&event) != 0)
//             {
//                 if (event.type == SDL_QUIT)
//                 {
//                     running = SDL_FALSE;
//                 }
//             }
//             for (i = 0; i < samplesPerFrame; i++)
//                 audioBuffer[audioMainLeftOff + i] = 0;
//             speak(&testVoiceA);
//             speak(&testVoiceB);
//             speak(&testVoiceC);
//             if (audioMainAccumulator > 1)
//             {
//                 for (i = 0; i < samplesPerFrame; i++)
//                 {
//                     audioBuffer[audioMainLeftOff + i] /= audioMainAccumulator;
//                 }
//             }
//             audioMainAccumulator = 0;
//             audioMainLeftOff += samplesPerFrame;
//             if (audioMainLeftOff == audioBufferLength)
//                 audioMainLeftOff = 0;
//             mainAudioLead = audioMainLeftOff - SDL_AtomicGet(&audioCallbackLeftOff);
//             if (mainAudioLead < 0)
//                 mainAudioLead += audioBufferLength;
//             if (mainAudioLead < floatStreamLength)
//                 printf("An audio collision may have occured!\n");
//             SDL_Delay(mainAudioLead * syncCompensationFactor);
//         }
//     }
// };

int main(int argc, char *argv[])
{
    // WY_Audio *wyAudio = new WY_Audio();
    // wyAudio->play();
    // wyAudio->loop();

    Game *game = new Game("0x06 font sprite", 256, 224, 3);
    game->run();

    // WY_Audio *wyAudio = new WY_Audio();
    // wyAudio->setup();
    // if (init())
    //     return 1;
    // wyAudio->play();
    // onExit();

    return 0;
}