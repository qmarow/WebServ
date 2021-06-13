#ifndef CGI_HPP
#define CGI_HPP

class CGI;

#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
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
    typedef struct sockaddr_in  sockaddr_t;

private:
    vector_string   _env;
    vector_string   _argv;
    string          _filename_script;
    string          _expansion;
    string          _url_cgi_file;
    URL             _url;
    Server          _server;
    Request         _request;
    sockaddr_t      _data_socket_client;

public:
    CGI();
    CGI(CGI const &x);
    CGI &operator=(CGI const &x);
    ~CGI();

    void    set_url(URL url);
    void    set_server(Server const &x);
    void    set_request(Request const &x);
    void    set_expansion(string expansion);
    void    set_url_cgi_file(string cgi_file);
    void    set_filename_script(string filename_script);
    void    set_data_socket_client(sockaddr_t data_socket_client);
    string  start();

private:
    void    set_env();
    char    **convert_array_string_to_char(vector_string array);
    void    free_array_char(char **array, int size);

    string  definition_auth_type(void);
    string  definition_content_length(void);
    string  definition_content_type(void);
    string  definition_gateway_interface(void);
    string  definition_path_info(void);
    string  definition_path_translated(void);
    string  definition_query_string(void);
    string  definition_remote_addr(void);
    string  definition_remote_ident(void);
    string  definition_remote_user(void);
    string  definition_request_method(void);
    string  definition_request_uri(void);
    string  definition_script_name(void);
    string  definition_server_name(void);
    string  definition_server_port(void);
    string  definition_server_protocol(void);
    string  definition_server_software(void);
};

#endif
