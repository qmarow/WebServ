#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP

#include <iostream>
#include <vector>
#include "../utils/File.hpp"
#include "./../utils/Structs.hpp"

#define COUNT_ERROR_PAGE 24

class ErrorPage
{
public:
    ErrorPage();
    ~ErrorPage();
    ErrorPage(ErrorPage const &x);
    ErrorPage   &operator=(ErrorPage const &x);

private:
    static int                  _codes_errors[COUNT_ERROR_PAGE];
    static std::string          _codes_names[COUNT_ERROR_PAGE];
    std::vector<t_error_page>   _errors_page_config;
    std::string                 _root_error_page;
    int                         _code_error;

public:
    std::string get_error_page(void);
    std::string get_name_error(void);
    int         get_code_error(void);
    void    set_code_error(int code_error);
    void    set_error_pages(std::vector<t_error_page> errors_page_config);
    void    set_root_error_page(std::string root);

private:
    std::string create_error_page(int code_error, std::string name_error);
};

#endif
