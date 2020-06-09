// 2020-06-09
// - last tested on wyngine repo, as experiment for push-audio
// - tested and worked on windows and web
// - Depending on AudioSettings.samples, delay and stutter may differ

// https://codereview.stackexchange.com/questions/41086/play-some-sine-waves-with-sdl2
// https://davidgow.net/handmadepenguin/ch7.html

// Audio programming theory
// https://www.youtube.com/watch?v=GjmcXfgKq78

// Music note frequencies
// https://pages.mtu.edu/~suits/notefreqs.html

// NES audio channels
// https://www.youtube.com/watch?v=la3coK5pq5w

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define BASE_FREQ_A0 27.50f

enum WY_AudioNote
{
    NOTE_A,
    NOTE_AS,
    NOTE_B,
    NOTE_C,
    NOTE_CS,
    NOTE_D,
    NOTE_DS,
    NOTE_E,
    NOTE_F,
    NOTE_FS,
    NOTE_G,
    NOTE_GS,
};

// void audioCallback(void *userData, Uint8 *stream, int streamLen);

class WY_Audio
{
    SDL_AudioDeviceID deviceId;

public:
    SDL_AudioSpec wantSpec;
    SDL_AudioSpec haveSpec;

    // Frequency, a.k.a. number of samples per second. Higher value = higher accuracy
    int mSampleRate;
    // Current sample position (audio dTime)
    int mSampleIndex = 0;

    // Buffer size for samples per channel. More samples = better accuracy, but slower performance.
    int mSampleSize;

    // Number of channels, e.g. mono = 1, stereo = 2, etc.
    int mChannels;

    // Volume. 0 = silence, 1000 = max
    int mAmplitude;

    WY_AudioNote mNote = NOTE_A;
    int mOctave = 4;
    float mPlaying = 0.f;

    /**
     * Determines sampleSize format (range). Larger types = larger sample range.
     * If the format is too large, you may end up hearing nothing because the
     * buffer data's values are all too low to be audible. The inverse is also true.
     *
     * i.e.: Lower format gives off a DOS-like sound, higher format is smoother and modern.
    */
    SDL_AudioFormat audioFormat = AUDIO_S16;

    WY_Audio(unsigned int sampleRate = 44100, unsigned int sampleSize = 1024, unsigned int channels = 1, unsigned int amplitude = 500)
    {
        SDL_Init(SDL_INIT_AUDIO);

        testInit();

        // SDL_zero(wantSpec);
        // SDL_zero(haveSpec);

        // wantSpec.freq = sampleRate;
        // wantSpec.format = audioFormat;
        // wantSpec.channels = channels;
        // wantSpec.samples = sampleSize;
        // wantSpec.callback = NULL;
        // wantSpec.userdata = NULL;
        // // wantSpec.callback = audioCallback; // not efficient for emscripten
        // // wantSpec.userdata = this;          // not efficient for emscripten

        // deviceId = SDL_OpenAudioDevice(NULL, 0, &wantSpec, &haveSpec, 0);
        // if (deviceId == 0)
        // {
        //     printf("\nFailed to open audio: %s\n", SDL_GetError());
        //     return;
        // }
        // if (wantSpec.format != haveSpec.format)
        //     printf("\nFailed to get the desired AudioSpec");

        // mSampleRate = haveSpec.freq;
        // mSampleSize = haveSpec.size / 4;
        // mChannels = channels;
        // mAmplitude = amplitude;

        play();
    }

    ~WY_Audio()
    {
        SDL_CloseAudioDevice(deviceId);

        delete &wantSpec;
        delete &haveSpec;
    }

    // Returns note in frequency
    double getNote()
    {
        double d12thRootOf2 = pow(2.0, 1.0 / 12);
        double octave = pow(2.0, mOctave);
        double res = BASE_FREQ_A0 * pow(d12thRootOf2, mNote) * (double)octave;

        return res;
    }

    void increaseOctave()
    {
        if (++mOctave > 7)
        {
            mOctave = 7;
        }
    }

    void decreaseOctave()
    {
        if (--mOctave < 1)
        {
            mOctave = 1;
        }
    }

    void speak(WY_AudioNote note)
    {
        mNote = note;
        mPlaying = 1.f;
    }

    void silence()
    {
        mPlaying = 0.f;
    }

    void play()
    {
        SDL_PauseAudioDevice(deviceId, 0);
    }

    void pause()
    {
        SDL_PauseAudioDevice(deviceId, 1);
    }

    // Overwrite this to create your own audio sample.
    // Do not printf here as it will be very slow;
    // It runs at a high frequency, e.g. ~44100 per frame
    // Expects a return value between -1 to 1.
    virtual double getAudioSample(double time)
    {
        return std::sin(2.0f * M_PI * time * mNote);
    }

    // virtual void updateAudio(Uint8 *stream, int streamLen)
    // {
    //     // TODO: probably need to move this into update loops
    //     // to fix stutters in web build

    //     // printf("\nsample: %d, %d", mSampleIndex, streamLen);

    //     Sint16 *buffer = (Sint16 *)stream;

    //     /**
    //      * stream length is (sampleSize * channels * byte format).
    //      *
    //      * e.g.
    //      * sampleSize = 1024
    //      * channels = 2
    //      * audioFormat = AUDIO_S16
    //      * streamLen = 1024 * 2 * 2 (S16 = 2 bytes) = 4096
    //      */
    //     int bufferLength = streamLen / 2; // 2 bytes per sample for AUDIO_S16SYS

    //     for (int i = 0; i < bufferLength; i++)
    //     {
    //         double samplePos = (double)mSampleIndex / (double)mSampleRate;
    //         buffer[i] = mPlaying * mAmplitude * (double)getAudioSample(samplePos);

    //         mSampleIndex++;
    //         // mSampleIndex %= mSampleRate;
    //     }
    // }

    SDL_AudioSpec AudioSettings = {0};
    int SamplesPerSecond = 48000;
    int ToneHz = 256;
    int16_t ToneVolume = 3000;
    uint32_t RunningSampleIndex = 0;
    int SquareWavePeriod = SamplesPerSecond / ToneHz;
    int HalfSquareWavePeriod = SquareWavePeriod / 2;
    int BytesPerSample = sizeof(int16_t) * 2;
    int BytesToWrite = 800 * BytesPerSample; // BufferSize

    void testInit()
    {
        AudioSettings.freq = SamplesPerSecond;
        AudioSettings.format = AUDIO_S16;
        AudioSettings.channels = 2;
        AudioSettings.samples = 4096;

        deviceId = SDL_OpenAudioDevice(NULL, 0, &AudioSettings, NULL, 0);

        SDL_PauseAudioDevice(deviceId, 0);

        printf("\ninit: %d", deviceId);
    }

    void update()
    {
        int TargetQueueBytes = 48000 * BytesPerSample;
        int BytesToWrite2 = TargetQueueBytes - SDL_GetQueuedAudioSize(deviceId);

        void *SoundBuffer = malloc(BytesToWrite2);
        int16_t *SampleOut = (int16_t *)SoundBuffer;
        int SampleCount = (BytesToWrite2 / BytesPerSample);
        int16_t queued = SDL_GetQueuedAudioSize(deviceId);

        printf("\nupdate: %d %d %d", BytesToWrite, BytesToWrite2, queued);

        for (int SampleIndex = 0;
             SampleIndex < SampleCount;
             ++SampleIndex)
        {
            int16_t SampleValue = mAmplitude * ((RunningSampleIndex++ / HalfSquareWavePeriod) % 2) ? ToneVolume : -ToneVolume;
            *SampleOut++ = SampleValue;
            *SampleOut++ = SampleValue;
        }

        SDL_QueueAudio(deviceId, SoundBuffer, BytesToWrite2);
        free(SoundBuffer);
    }

    // void update()
    // {
    //     Uint16 bytesRemaining = SDL_GetQueuedAudioSize(deviceId);
    //     printf("\n%d", bytesRemaining);

    //     if (bytesRemaining < 4096)
    //     {
    //         Sint16 data[4096];

    //         for (int i = 0; i < 4096; i++)
    //         {
    //             double samplePos = (double)mSampleIndex / (double)mSampleRate;
    //             data[i] = mAmplitude * (double)getAudioSample(samplePos);

    //             mSampleIndex++;
    //         }

    //         SDL_QueueAudio(deviceId, data, 4096);
    //     }
    // }
};

// void audioCallback(void *user_data, Uint8 *raw_buffer, int bytes)
// {
//     static_cast<WY_Audio *>(user_data)->updateAudio(raw_buffer, bytes);
// }
