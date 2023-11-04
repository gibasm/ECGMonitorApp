#include "arg_parser.hh"
#include <cstring>

namespace
ecgm 
{

static int 
find_opt_pos(const char* opt, int argc, char** argv)
{
    for(int i = 1; i < argc; ++i)
    {
        if(strcmp(argv[i], opt) == 0)
            return i;
    } 

    return -1;
}

bool 
get_bool_opt(const char* opt, int argc, char** argv)
{
    return find_opt_pos(opt, argc, argv) != -1;      
}

char* 
get_str_opt(const char* opt, int argc, char** argv)
{
    int ret = find_opt_pos(opt, argc, argv);

    if((ret == -1) || (ret == (argc-1)))
        return nullptr;
    
    return argv[ret+1];
}

}
