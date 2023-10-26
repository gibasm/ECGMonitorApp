#include "log.hh"
#include <cstdio>
#include <ctime>
#include <cstdarg>

namespace
ecgm
{

std::atomic<log_lvl> __log_lvl = STATUS;

inline static void 
log_w_tag(const char* tag, const char* format, va_list args)
{
    auto t = std::time(nullptr);
    auto local_t = std::localtime(&t);

    printf("[%02d:%02d:%02d][%s] \t", 
            local_t->tm_hour,
            local_t->tm_min,
            local_t->tm_sec, 
            tag);

    vprintf(format, args); 

    putc('\n', stdout);
}


#define DECL_LOG_FUNC(name, lvl) \
void \
log_ ## name (const char* format, ...) \
{ \
    if(__log_lvl < lvl) \
        return; \
    va_list args; \
    va_start(args, format); \
    log_w_tag(#name, format, args); \
    va_end(args); \
} 

DECL_LOG_FUNC(error, log_lvl::ERROR)
DECL_LOG_FUNC(warning, log_lvl::WARNING)
DECL_LOG_FUNC(status, log_lvl::STATUS)
DECL_LOG_FUNC(debug, log_lvl::DEBUG)

void 
set_log_lvl(log_lvl lvl)
{
    __log_lvl = lvl;
}

};
