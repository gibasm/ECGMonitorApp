#ifndef LOG_HH
#define LOG_HH

#include <atomic>

namespace
ecgm
{

enum 
log_lvl
{
    SILENT = 0,
    ERROR, 
    WARNING,
    STATUS,
    DEBUG
};


void
log_error(const char* format, ...);

void
log_warning(const char* format, ...);

void
log_status(const char* format, ...);

void
log_debug(const char* format, ...);

void 
set_log_lvl(log_lvl lvl);

extern std::atomic<log_lvl> __log_lvl;

};

#endif /* LOG_HH */
