#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>

typedef struct  s_error_page {
    int         code;
    std::string file;
}               t_error_page;

typedef struct  s_redirect {
    int         code;
    std::string url;
}               t_redirect;

#endif