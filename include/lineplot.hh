#ifndef LINEPLOT_HH
#define LINEPLOT_HH

#include <iterator>
#include <memory>
#include <type_traits>
#include <SDL2/SDL.h>
#include "render_target.hh"
#include "render_context.hh"

namespace
ecgm
{

template <std::random_access_iterator it_t>
class
lineplot : public ecgm::render_target
{
public:
    lineplot(it_t begin, it_t end, render_context* context);

    virtual 
    ~lineplot() = default;

    virtual void 
    render() override;

private:
    std::unique_ptr<SDL_Point[]> points;
    size_t n_points;
    
    SDL_Point ybaseline[2];

    render_context* context;
};

}

#endif /* LINEPLOT_HH */
