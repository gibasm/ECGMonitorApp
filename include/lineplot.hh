#ifndef LINEPLOT_HH
#define LINEPLOT_HH

#include <iterator>
#include <atomic>
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
lineplot : public ecgm::render_target, public ecgm::observer
{
public:
    lineplot(it_t begin, it_t end, render_context* context);

    virtual 
    ~lineplot() = default;

    virtual void 
    render() override;

    virtual void
    notify(void*) override;

private:
    std::unique_ptr<SDL_Point[]> points;
    size_t n_points;
    
    SDL_Point ybaseline[2];

    render_context* context;

    int ybase, ydev, xstep;
    
    std::atomic_bool needs_full_coord_recalc;
    std::atomic_bool needs_point_recalc;
    size_t point_idx;

    void
    calculate_base_coords(const render_context_dimensions&);

    void
    recalculate_point_yval();

    void
    recalculate_plot_coords();

    it_t series_begin, series_end;
};

}

#endif /* LINEPLOT_HH */
