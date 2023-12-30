#ifndef LINEPLOT_HH
#define LINEPLOT_HH

#include <iterator>
#include <vector>
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

    void 
    set_xticks(int xtick);

    void
    set_yticks(int ytick);

private:
    std::unique_ptr<SDL_Point[]> points;
    size_t n_points;
    
    SDL_Point ybaseline[2];

    render_context* context;

    int ybase, ydev, xstep;

    std::mutex notify_lock;    
    size_t point_idx;

    void
    calculate_base_coords(const render_context_dimensions&);

    void
    recalculate_point_yval();

    void
    recalculate_plot_coords();

    it_t series_begin, series_end;

    std::vector<SDL_Point> xticks = {}, yticks = {};
};

}

#endif /* LINEPLOT_HH */
