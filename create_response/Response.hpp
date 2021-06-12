#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "./../parse_request/Request.hpp"
#include "./../parse_confige/Server.hpp"
#include "./../utils/UtilsTime.hpp"
#include "./ContentType.hpp"
#include "./ErrorPage.hpp"

class ErrorPage;

class Response{
public:
    typedef std::string string;
    Response();
    ~Response();
    Response(Response const &x);
    Response    &operator=(Response const &x);

private:
    string      _response;
    int         _code_status;
    string      _body_message;
    string      _url;
    string      _expansion;
    Request     _request;
    Server      _server;
    string      _redirect;
    ErrorPage   _error_page;
    bool        _is_authorization;

public:
    string  get_response(void);
    void    set_authorization(bool auth);
    void    set_body_message(const string &body_message);
    void    set_code_status(int code_status);
    void    set_request(const Request &request);
    void    set_error_page(ErrorPage &error_page);
    void    set_url(const string &url);
    void    set_url_redirect(const string &redirect);
    void    set_expansion(const string &expansion); // expansion - расширение

    int     get_code_status(void);
    string  get_body_messagee(void);
    string  get_error_page(void);

private:
    string shape_the_response(void);
    string create_headers(void);
    string get_body_message(void);
    string header_data(void);
    string header_server(void);
    string header_last_modified(string name_file);
    string header_content_lenght(void);
    string header_ContentType(void);
    string header_content_language(void);
    string header_allow(void);
    string header_content_location(void);
    string header_location(void);
    string header_retry_after(void);
    string header_www_authenticate(void);

    // void header_for_GET(void);
    // void header_for_HEAD(void);
    // void header_for_POST(void);
    // void header_for_PUT(void);
};

#endif