#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include <string>
#include <unistd.h>
#include <vector>
#include <dirent.h>
#include <fcntl.h>
#include "./../utils/UtilsString.hpp"

class Autoindex {
public:
    typedef std::string                 string;
    typedef std::pair<string, string>   pair;
    typedef std::vector<pair>           vector_pair;
    typedef std::vector<string>         vector_string;

private:
    string _url;
    string _root;
    string _directory;

public:
    Autoindex();
    Autoindex(const Autoindex &autoindex);
    Autoindex &operator=(const Autoindex &autoindex);
    ~Autoindex();

    void set_root(string root);
    void set_directory(string directory);
    void set_url_location(string url);

    string get_html();

    class NotFoundException : std::exception {
    public:
        const char * what() const throw();
    };

private:
    vector_pair     get_directories(void);
    string          get_href(string object);
    string          get_href_back(string url_location, string path_directory);
    vector_string   get_directory_names(void);
    string          get_html_directories(vector_pair directories);
    string          get_html_title(void);
    string          get_html_directory(pair directory);
};

#endif