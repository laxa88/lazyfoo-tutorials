#include <random>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

unsigned char random(int mod)
{
#ifdef __EMSCRIPTEN__
    return emscripten_random() * mod;
#else
    return rand() % mod;
#endif
}