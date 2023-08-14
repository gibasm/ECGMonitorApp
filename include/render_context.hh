#ifndef RENDER_CONTEXT_HH
#define RENDER_CONTEXT_HH

#include <atomic>
#include <SDL2/SDL.h>

namespace
ecgm
{

typedef struct
{
    SDL_Renderer** renderer;
    std::atomic_int xoff, yoff;
    std::atomic_int width, height;
} render_context;

}

#endif /* RENDER_CONTEXT_HH */
