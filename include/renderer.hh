#ifndef RENDERER_HH
#define RENDERER_HH

#include <list>
#include <SDL2/SDL_render.h>
#include "render_target.hh"


namespace
ecgm
{

typedef 
std::list<render_target_ptr> render_list_t;

typedef struct
{
    render_list_t* render_targets; 
    const char* window_title;
    int window_width, window_height;
} renderer_args_t;

void 
renderer_thread_start(renderer_args_t* args);

extern SDL_Renderer* renderer;

}


#endif /* RENDERER_HH */
