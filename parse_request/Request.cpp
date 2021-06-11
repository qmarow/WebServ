#include "Request.hpp"

// PUBLIC

Request::Request() {
    _method = string();
    _url_string = string();
    _keys_headers = vector_string();
    _headers = map_headers();
    _body = string();
}

Request::Request(const Request &data_request) {
    *this = data_request;
}

Request &Request::operator=(const Request &data_request) {
    _method = data_request._method;
    _url_string = data_request._url_string;
    _keys_headers = data_request._keys_headers;
    _headers = data_request._headers;
    _body = data_request._body;
    return (*this);
}

Request::~Request() {}

void Request::set_method(string method) {
    _method = method;
}

void Request::set_url(string directory) {
    _url_string = directory;
}

void Request::set_body(string body) {
    _body += body;
}

void Request::set_header(string key_header, vector_string values) {
    _keys_headers.push_back(key_header);
    _headers[key_header] = values;
}

bool Request::is_header(string key_header) {
    return (find_word(_keys_headers, key_header) != -1);
}

std::string Request::get_method(void) {
    return (_method);
}

std::string Request::get_url_string(void) {
    std::cout << "******\n";
    return (_url_string);
}

std::string Request::get_body(void) {
    return (_body);
}

std::vector<std::string> Request::get_keys_headers(void) {
    return (_keys_headers);
}

std::vector<std::string> Request::get_values_header(string key_header) {
    return (_headers[key_header]);
}