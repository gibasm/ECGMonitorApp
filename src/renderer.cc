#include "renderer.hh"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <SDL2/SDL.h>
#include "app_globl_state.hh"

namespace
ecgm
{

SDL_Renderer* renderer;

void 
renderer_thread_start(renderer_args_t* args)
{
    SDL_Event event;
 
    SDL_Init(SDL_INIT_VIDEO);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");   
 
    SDL_Window* window = SDL_CreateWindow(
        args->window_title,
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        args->window_width, 
        args->window_height, 
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    if(window == NULL)
    {
        printf("%s\n", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if(renderer == NULL)
    {
        printf("%s\n", SDL_GetError());
        return;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    while (!app_state.should_terminate)
    {
        SDL_PollEvent(&event);
 
        switch (event.type)
        {
            case SDL_QUIT:
                app_state.should_terminate = true; 
                break;
        }

        for(auto& render_target : *(args->render_targets)) {
            render_target->render();
        }

        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }
 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

}
