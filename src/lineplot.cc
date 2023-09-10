#include "lineplot.hh"
#include <memory>
#include <vector>
#include <iterator>
#include <cmath>

namespace
ecgm
{

template <std::random_access_iterator it_t>
lineplot<it_t>::lineplot(it_t begin, it_t end, render_context* context)
:context(context), series_begin(begin), series_end(end)
{
    this->n_points = std::distance(begin, end);
    this->points = std::make_unique<SDL_Point[]>(this->n_points); 
    this->recalculate_plot_coords();

    this->context->add_observer(this);
}

template<std::random_access_iterator it_t>
void
lineplot<it_t>::calculate_base_coords(const render_context_dimensions& dimensions)
{

    this->xstep = static_cast<int>(std::round(dimensions.width / static_cast<float>(this->n_points))); 
    this->ydev = (dimensions.height / 2);

    this->ybase = this->ydev + dimensions.yoff;

    this->ybaseline[0] = {
        .x = dimensions.xoff, 
        .y = ybase 
    };

    this->ybaseline[1] = {
        .x = dimensions.xoff + dimensions.width,
        .y = ybase
    };   
}

template<std::random_access_iterator it_t>
void
lineplot<it_t>::recalculate_plot_coords()
{
    auto dimensions = this->context->get_dimensions();

    this->calculate_base_coords(dimensions);

    SDL_Point point;
    point.x = (this->xstep / 2) + dimensions.xoff;  

    for(auto it = series_begin; it != series_end; ++it) 
    {
        point.y = ybase - static_cast<int>(*it * this->ydev);
        this->points[std::distance(series_begin, it)] = point;
        point.x += xstep;
    } 
}

template<std::random_access_iterator it_t>
void
lineplot<it_t>::recalculate_point_yval()
{
    SDL_Point point = this->points[point_idx]; 

    auto series_item = series_begin + point_idx;
    
    point.y = ybase - static_cast<int>(*series_item * this->ydev);

    this->points[point_idx] = point;
}

template<std::random_access_iterator it_t>
void
lineplot<it_t>::render() 
{
    SDL_SetRenderDrawColor(*(this->context->get_renderer()), 0xFF, 0x77, 0x77, 0xFF);
    SDL_RenderDrawLines(*(this->context->get_renderer()), this->points.get(), this->n_points); 
    SDL_SetRenderDrawColor(*(this->context->get_renderer()), 0xFF, 0xFF, 0xFF, 0x60);
    SDL_RenderDrawLines(*(this->context->get_renderer()), this->ybaseline, 2); 
    SDL_SetRenderDrawColor(*(this->context->get_renderer()), 0x00, 0x00, 0x00, 0x00);

    if(this->needs_full_coord_recalc)
    {
        this->recalculate_plot_coords();
        this->needs_full_coord_recalc = false; 
    }

    if(this->needs_point_recalc)
    {
        this->recalculate_point_yval();
        this->needs_point_recalc = false;
    }
}

template<std::random_access_iterator it_t>
void
lineplot<it_t>::notify(void* arg)
{
    if(arg == nullptr)
    {
        this->needs_full_coord_recalc = true;
        return;
    } 

    point_idx = *(int*)(arg);

    this->needs_point_recalc = true;
}

template class lineplot<std::vector<float>::iterator>;

}
