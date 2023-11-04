#ifndef ARG_PARSER_HH
#define ARG_PARSER_HH

namespace
ecgm
{

bool 
get_bool_opt(const char* opt, int argc, char** argv);

char* 
get_str_opt(const char* opt, int argc, char** argv);

}

#endif /* ARG_PARSER_HH */
