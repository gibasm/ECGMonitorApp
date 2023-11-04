#ifndef APP_GLOBL_STATE_HH
#define APP_GLOBL_STATE_HH

#include <atomic>

namespace
ecgm
{

typedef struct
{
    std::atomic_bool should_terminate;
} app_state_t;

extern app_state_t app_state;

}

#endif /* APP_GLOBL_STATE_HH */
