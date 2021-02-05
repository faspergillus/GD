#include <SDL2/SDL_version.h>


int main(int /*argc*/, char* /*argv*/[])
{
    SDL_version compiled;
    SDL_version linked;

// SDL_GetVersion function returns the current version, while SDL_VERSION() is a macro that tells you what version you compiled with.
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    printf("We compiled SDL version %d.%d.%d.\n",
           compiled.major, compiled.minor, compiled.patch);
    printf("We are linking SDL version %d.%d.%d.\n",
           linked.major, linked.minor, linked.patch);
}

