#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "./../utils/UtilsTime.hpp"
#include "./Content_type.hpp"
#include "./ErrorPage.hpp"
#include "./../header.hpp"

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

public:
    string  get_response();
    void    set_body_message(string const &body_message);
    void    set_code_status(int code_status);
    void    set_request(Request const &request);
    void    set_url(string const &url);
    void    set_url_redirect(const string &redirect);
    void    set_expansion(string const &expansion); // expansion - расширение
private:
    string create_header();
    string header_data();
    string header_server();
    string header_last_modified(string name_file);
    string header_content_lenght();
    string header_content_type();
    string header_content_language();
    string header_allow();
    string header_content_location();
    string header_location();
    string header_retry_after();

    void header_for_GET();
    void header_for_HEAD();
    void header_for_POST();
    void header_for_PUT();
    void header_for_DELETE();
    // void header_for
};

#endif