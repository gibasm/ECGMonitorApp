#include "lineplot.hh"
#include <SDL_render.h>
#include <memory>
#include <vector>
#include <iterator>
#include <cmath>
#include "log.hh"

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

    log_debug("UI: lineplot: point redraw request (%d, %0.2f)", point_idx, *series_item);

    this->points[point_idx] = point;
}

template<std::random_access_iterator it_t>
void
lineplot<it_t>::render() 
{
    notify_lock.lock();
    SDL_SetRenderDrawColor(*(this->context->get_renderer()), 0xFF, 0x77, 0x77, 0xFF);
    SDL_RenderDrawLines(*(this->context->get_renderer()), this->points.get(), this->n_points); 
    SDL_SetRenderDrawColor(*(this->context->get_renderer()), 0xFF, 0xFF, 0xFF, 0x60);
    SDL_RenderDrawLines(*(this->context->get_renderer()), this->ybaseline, 2); 

    for(size_t i = 0; i < xticks.size(); i += 2)
        SDL_RenderDrawLines(*(this->context->get_renderer()), &(xticks.data()[i]), 2);

    for(size_t i = 0; i < yticks.size(); i += 2)
        SDL_RenderDrawLines(*(this->context->get_renderer()), &(yticks.data()[i]), 2);

    SDL_SetRenderDrawColor(*(this->context->get_renderer()), 0x00, 0x00, 0x00, 0x00);
    notify_lock.unlock();
}

template<std::random_access_iterator it_t>
void
lineplot<it_t>::set_xticks(int xtick)
{
    size_t n_ticks = (2 * std::distance(series_begin, series_end)) / xtick;
    n_ticks += (n_ticks % 2) != 0 ? 1 : 0;

    xticks.resize(n_ticks);

    int xpos = this->ybaseline[0].x;
    for(size_t i = 0; i < xticks.size(); i += 2)
    {
        xticks.at(i) = {
            xpos, this->ybaseline[0].y + 10
        };

        xticks.at(i + 1) = {
            xpos, this->ybaseline[0].y - 10
        };
        xpos += xtick;
    }
}

template<std::random_access_iterator it_t>
void
lineplot<it_t>::set_yticks(int ytick)
{
    size_t n_ticks = (4 * ydev) / ytick;
    n_ticks += (n_ticks % 2) != 0 ? 1 : 0;
      
    yticks.resize(n_ticks);

    int ypos = this->ybaseline[0].y - ydev;
    for(size_t i = 0; i < yticks.size(); i += 2)
    {
        yticks.at(i) = {
            this->ybaseline[0].x - 10, ypos 
        }; 

        yticks.at(i + 1) = {
            this->ybaseline[0].x + 10, ypos
        }; 
        
        ypos += ytick;
    }

}

template<std::random_access_iterator it_t>
void
lineplot<it_t>::notify(void* arg)
{
    if(arg == nullptr)
    {
        notify_lock.lock();
        this->recalculate_plot_coords();
        notify_lock.unlock();
        log_debug("UI: lineplot: received notification for full coord recalc");    
        return;
    } 

    point_idx = *(int*)(arg);
    log_debug("UI: lineplot: received notification with argument %d", point_idx);    

    notify_lock.lock();
    this->recalculate_point_yval();
    notify_lock.unlock();
}

template class lineplot<std::vector<float>::iterator>;

}
