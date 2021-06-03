#ifndef CGI_HPP
#define CGI_HPP

#include <vector>
#include <string>
#include <unistd.h>
#include "./../parse_url/URL.hpp"
#include "./../parse_confige/Server.hpp"
#include "./../parse_request/Request.hpp"
#include "./../handler_connects/Client.hpp"
#include "./../utils/UtilsString.hpp"

class CGI
{
public:
    typedef std::string string;
    typedef std::vector<string> vector_string;

    CGI();
    ~CGI();
    CGI(CGI const &x);
    CGI     &operator=(CGI const &x);
private:
    char           **_env;
    char           **_argv;
    string          _filename_script;
    string          _expansion;
    string          _url_cgi_file;
    URL             _url;
    Server          _server;
    Request         _request;
    Client          _client;
public:
    void    set_url(URL url);
    void    set_server(Server const &x);
    void    set_request(Request const &x);
    void    set_expansion(string expansion);
    void    set_url_cgi_file(string cgi_file);
    void    set_filename_script(char *filename_script);
    char    *start_cgi();
private:
    void    set_env();
    char    *copy_string_to_char(string s1, char *s2);
    char    **add_and_relocation(char **base, char *s_add);

    char    *definition_auth_type();
    char    *definition_content_length();
    char    *definition_content_type();
    char    *definition_gateway_interface();
    char    *definition_path_info();
    char    *definition_path_translated();
    char    *definition_query_string();
    char    *definition_remote_addr();
    char    *definition_remote_ident();
    char    *definition_remote_user();
    char    *definition_request_method();
    char    *definition_request_uri();
    char    *definition_script_name();
    char    *definition_server_name();
    char    *definition_server_port();
    char    *definition_server_protocol();
    char    *definition_server_software();
};

#endif
