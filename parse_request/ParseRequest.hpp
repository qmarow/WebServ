#ifndef PARSEREQUEST_HPP
#define PARSEREQUEST_HPP

#define COUNT_KEY_HEADERS 18
#define COUNT_TYPE_REQUESTS 5

#include <vector>
#include <string>
#include <map>
#include "Request.hpp"
#include "./../utils/UtilsString.hpp"

class ParseRequest {
public:
    typedef std::string                     string;
    typedef std::vector<string>             vector_string;
    typedef std::map<string, vector_string> map_headers;

private:
    static const char *_headers[COUNT_KEY_HEADERS];
    static const char *_type_requests[COUNT_TYPE_REQUESTS];
    Request _request;

public:
    ParseRequest();
    ParseRequest(const ParseRequest &parse_request);
    ParseRequest &operator=(const ParseRequest &parse_request);
    ~ParseRequest();

    Request get_request(void);
    void open(string request);

    class MissingStartLineException: public std::exception {
    public:
        const char* what() const throw();
    };

    class MissingRequiredHeadersException: public std::exception {
    public:
        const char* what() const throw();
    };

    class MissingBodyException: public std::exception {
    public:
        const char* what() const throw();
    };

private:
    void parse_starting_line(string starting_line);
    void parse_headers(vector_string headers);
    void parse_body(string request);

    bool is_correct_header(string header);
    bool is_correct_type_request(string type_request);
};

#endif