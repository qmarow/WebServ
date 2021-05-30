#ifndef URL_HPP
#define URL_HPP

#include <iostream>
#include <string>
#include "./../utils/UtilsString.hpp"

class URL {
public:
    typedef std::string         string;
    typedef std::vector<string> vector_string;

private:
    string          _path;
    vector_string   _keys_argv;
    vector_string   _values_argv;

public:
    URL();
    URL(const URL &url);
    URL &operator=(const URL &url);
    ~URL();

    void            parse(string url_string);
    int             count_argv(void);
    string          get_path(void);
    vector_string   get_keys_argv(void);
    vector_string   get_values_argv(void);
    string          at_keys(int index);
    string          at_values(int index);
};

#endif