#include "lineplot.hh"
#include <vector>
#include <iterator>
#include <cmath>

namespace
ecgm
{

template <std::random_access_iterator it_t>
lineplot<it_t>::lineplot(it_t begin, it_t end, render_context* context)
:context(context)
{
    this->n_points = std::distance(begin, end);
 
    int xstep = static_cast<int>(std::round(this->context->width / static_cast<float>(this->n_points))); 
    int ydeviation = (this->context->height / 2);
    int ybase = ydeviation + this->context->yoff;

    this->points = std::make_unique<SDL_Point[]>(this->n_points);

    SDL_Point point;
    point.x = (xstep / 2) + this->context->xoff;  

    for(auto it = begin; it != end; ++it) 
    {
        point.y = ybase - static_cast<int>(*it * ydeviation);
        this->points[std::distance(begin, it)] = point;
        point.x += xstep;
    } 

    ybaseline[0] = {
        .x = this->context->xoff, 
        .y = ybase 
    };

    ybaseline[1] = {
        .x = this->context->xoff + this->context->width,
        .y = ybase
    };
}

template<std::random_access_iterator it_t>
void
lineplot<it_t>::render() {
    SDL_SetRenderDrawColor(*(this->context->renderer), 0xFF, 0x77, 0x77, 0xFF);
    SDL_RenderDrawLines(*(this->context->renderer), this->points.get(), this->n_points); 
    SDL_SetRenderDrawColor(*(this->context->renderer), 0xFF, 0xFF, 0xFF, 0x60);
    SDL_RenderDrawLines(*(this->context->renderer), this->ybaseline, 2); 
    SDL_SetRenderDrawColor(*(this->context->renderer), 0x00, 0x00, 0x00, 0x00);
}

template class lineplot<std::vector<float>::iterator>;

}
