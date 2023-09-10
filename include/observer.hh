#ifndef OBSERVER_HH
#define OBSERVER_HH

namespace 
ecgm
{

class 
observer
{
public:
    virtual void notify(void*) = 0;
};

}

#endif /* OBSERVER_HH */
