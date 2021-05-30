#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>
#include <map>
#include "./../utils/UtilsString.hpp"

class Request {
public:
    typedef std::string                     string;
    typedef std::vector<string>             vector_string;
    typedef std::map<string, vector_string> map_headers;

private:
    string          _method;
    string          _url_string;
    vector_string   _keys_headers;
    map_headers     _headers;
    string          _body;

public:
    Request();
    Request(const Request &data_request);
    Request &operator=(const Request &data_request);
    ~Request();

    void set_method(string method);
    void set_url(string url);
    void set_header(string key_header, vector_string values);
    void set_body(string body);

    bool is_header(string key_header);

    string          get_method(void);
    string          get_url_string(void);
    string          get_body(void);
    vector_string   get_keys_headers(void);
    vector_string   get_values_header(string key_header);
};

#endif