#ifndef RENDER_CONTEXT_HH
#define RENDER_CONTEXT_HH

#include <atomic>
#include <SDL2/SDL.h>
#include <forward_list>
#include "observer.hh"

namespace
ecgm
{

typedef struct
{
    int xoff, yoff;
    int width, height;
} render_context_dimensions;

class render_context
{
public:
    render_context(SDL_Renderer** renderer, int width, int height, int xoff, int yoff);

    SDL_Renderer** 
    get_renderer();

    void 
    add_observer(observer* obs);

    void
    set_width(int width);

    void
    set_height(int height);
    
    void
    set_xoff(int xoff);

    void 
    set_yoff(int yoff);
    
    render_context_dimensions
    get_dimensions();

private:
    void notify_observers();

    SDL_Renderer** renderer;

    std::atomic_int xoff, yoff;
    std::atomic_int width, height;

    std::forward_list<observer*> observers;
};

}

#endif /* RENDER_CONTEXT_HH */
