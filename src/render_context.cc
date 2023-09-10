#include "render_context.hh"

namespace ecgm
{


render_context::render_context(SDL_Renderer** renderer, int width, int height, int xoff, int yoff)
:renderer(renderer), xoff(xoff), yoff(yoff), width(width), height(height)
{
}

SDL_Renderer** 
render_context::get_renderer()
{
    return renderer;
}

void 
render_context::add_observer(observer* obs)
{
    observers.push_front(obs);
}

void
render_context::set_width(int width)
{
    this->width = width;
    notify_observers();
}

void
render_context::set_height(int height)
{
    this->height = height;
    notify_observers();
}

void
render_context::set_xoff(int xoff)
{
    this->xoff = xoff;
    notify_observers();
}

void 
render_context::set_yoff(int yoff)
{
    this->yoff = yoff;
    notify_observers();
}

render_context_dimensions
render_context::get_dimensions()
{
    return { xoff, yoff, width, height };
}

void
render_context::notify_observers()
{
    for(auto& obs : observers)
    {
        if(obs != nullptr)
            obs->notify(nullptr);
    }
}

}
