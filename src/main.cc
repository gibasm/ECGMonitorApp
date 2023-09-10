#include <cmath>
#include <cstdlib>
#include <thread>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <SDL2/SDL.h>
#include "renderer.hh"
#include "lineplot.hh"

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif /* EXIT_SUCCESS */

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif /* EXIT_FAILURE */

constexpr const char* window_title = "ECG Monitor"; 
constexpr const int initial_width = 1080;
constexpr const int initial_height = 720; 

namespace ecgm
{
extern SDL_Renderer* renderer;
}

int 
main(void)
{
    std::vector<float> test_series = { 0.0f, 0.0f, 0.0f, 00.0f, 00.0f, 0.0f, 1.0f, -1.0f, 0.0f, 00.0f, 00.0f, 0.0f, 0.0f, 0.0f };

    ecgm::render_list_t render_targets;

    ecgm::renderer_args_t renderer_args = {
        .render_targets = &render_targets,
        .window_title = window_title,
        .window_width = initial_width,
        .window_height = initial_height
    }; 

    ecgm::render_context main_plot_context(&ecgm::renderer, 1020, 360, 20, 200);

    ecgm::lineplot<std::vector<float>::iterator> plot(
            test_series.begin(), 
            test_series.end(), 
            &main_plot_context);

    render_targets.push_back(&plot); 

    std::jthread renderer_thread(ecgm::renderer_thread_start, &renderer_args);

    renderer_thread.join(); 

    return EXIT_SUCCESS;
}
