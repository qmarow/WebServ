#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include "./../utils/UtilsString.hpp"
#include "./../utils/Structs.hpp"


class Server {
public:
    typedef std::string                 string;
    typedef std::vector<string>         vector_string;
    typedef std::vector<int>            vector_int;
    typedef std::vector<Server>         vector_server;
    typedef std::vector<t_error_page>   vector_error_page;

private:
    string              _ip;
    int                 _port;
    int                 _max_body_size;
    string              _root;
    string              _server_name;
    vector_string       _index;
    bool                _autoindex;
    vector_error_page   _error_pages;
    vector_string       _keys_locations;
    vector_server       _locations;
    string              _cgi_pass;
    //cgi_param
    //internal
    t_redirect            _redirect;
    vector_string       _auth_basic;
    //auth_basic_user_file
    vector_string       _allow_methods;
    vector_string       _cgi_allow_methods;
    //default_method_body
    int                 _timeout;

public:
    Server();
    Server(const Server &parse_confige);
    Server &operator=(const Server &server);
    ~Server();

    void                copy_directive(const Server &server);

    string              get_ip(void);
    int                 get_port(void);
    int                 get_max_body_size(void);
    string              get_root(void);
    string              get_server_name(void);
    vector_string       get_index(void);
    string              get_file_error_page(int code_error);
    vector_int          get_cods_error_page(void);
    vector_error_page   get_error_pages(void);
    vector_string       get_auth_basic(void);
    int                 get_code_redirect(void);
    string              get_url_redirect(void);
    int                 get_timeout(void);
    vector_string       get_allow_methods(void);
    vector_string       get_cgi_allow_methods(void);
    string              get_cgi_pass(void);
    Server              &get_location(string key_location);
    Server              &get_location(int index);
    vector_string       &get_keys_locations(void);

    void            set_ip(string ip);
    void            set_port(int port);
    void            set_max_body_size(int max_body_size);
    void            set_root(string root);
    void            set_server_name(string server_name);
    void            set_index(vector_string index);
    void            set_autoindex(bool autoindex);
    void            set_error_page(vector_int cods, string file);
    void            set_location(string url, Server &location);
    void            set_redirect(int code, string url);
    void            set_auth_basic(vector_string auth_basic);
    void            set_timeout(int value);
    void            set_allow_methods(vector_string methods);
    void            set_deny_methods(vector_string methods);
    void            set_cgi_allow_methods(vector_string methods);
    void            set_cgi_deny_methods(vector_string methods);
    void            set_cgi_pass(string cgi_pass);

    bool            is_max_body_size(void);
    bool            is_root(void);
    bool            is_server_name(void);
    bool            is_index(void);
    bool            is_autoindex(void);
    bool            is_error_page(int code_error);
    bool            is_location(string key_location);
    bool            is_method(string method);
    bool            is_allow_methods(void);
    bool            is_cgi_method(string method);
    bool            is_cgi_allow_methods(void);
    bool            is_redirect(void);
    bool            is_timeout(void);
    bool            is_authorization(void);

};

#endif