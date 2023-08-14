#ifndef RENDER_TARGET_HH
#define RENDER_TARGET_HH

#include <memory>

namespace
ecgm
{

class 
render_target
{
public:
    render_target() = default;
    virtual 
    ~render_target() = default;
    
    virtual void 
    render() = 0;
};

typedef
render_target* render_target_ptr;

}

#endif /* RENDER_TARGET_HH */
