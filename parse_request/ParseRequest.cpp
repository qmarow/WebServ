#include "ParseRequest.hpp"

// PRIVATE

const char *ParseRequest::_headers[COUNT_KEY_HEADERS] = {"Accept-Charsets", "Accept-Language", "Allow", "Authorization",
                                                    "Content-Language", "Content-Length", "Content-Location",
                                                    "Content-Type", "Date", "Host", "Last-Modified",
                                                    "Location", "Referer", "Retry-After", "Server",
                                                    "Transfer-Encoding", "User-Agent", "WWW-Authenticate"};

const char *ParseRequest::_type_requests[COUNT_TYPE_REQUESTS] = {"GET", "HEAD", "POST", "PUT", "DELETE"};

// PUBLIC

ParseRequest::ParseRequest() {
    _request = Request();
}

ParseRequest::ParseRequest(const ParseRequest &parse_request) {
    *this = parse_request;
}

ParseRequest &ParseRequest::operator=(const ParseRequest &parse_request) {
    _request = parse_request._request;
    return (*this);
}

ParseRequest::~ParseRequest() {}

Request ParseRequest::get_request(void) {
    return (_request);
}

void ParseRequest::open(string request) {
    size_t          position_body = request.find("\r\n\r\n");

    if (position_body == std::string::npos) {
        position_body = request.size();
    }
    string          request_without_body = request.substr(0, position_body);
    vector_string   data = split_line(request_without_body, "\r\n");
    string          starting_line = data[0];
    vector_string   headers(data.begin() + 1, data.end());
    string          body = request.substr(position_body, request.size());

    parse_starting_line(starting_line);
    parse_headers(headers);
    parse_body(body);
}

const char *ParseRequest::MissingStartLineException::what() const throw() {
    return ("Missing start line!");
}

const char *ParseRequest::MissingRequiredHeadersException::what() const throw() {
    return ("Missing required headers!");
}

const char *ParseRequest::MissingBodyException::what() const throw() {
    return ("Missing body!");
}

// PRIVATE

void ParseRequest::parse_starting_line(string starting_line) {
    vector_string data = split_line(starting_line, " ");

    if (data.size() != 3) {
        throw MissingStartLineException();
    }
    if (is_correct_type_request(data[0]) == false) {
        throw MissingStartLineException();
    }
    if (data[2] != string("HTTP/1.1")) {
        throw MissingStartLineException();
    }
    _request.set_method(data[0]);
    _request.set_url(data[1]);
}

void ParseRequest::parse_headers(vector_string headers) {
    for (int i = 0; i < headers.size(); i++) {
        vector_string data = split_line(headers[i], ":, ");
        if (is_correct_header(data[0]) == false) {
            // throw MissingRequiredHeadersException();
            continue ;
        }
        vector_string values(data.begin() + 1, data.end());
        _request.set_header(data[0], values);
    }
}

void ParseRequest::parse_body(string body) {
    _request.set_body(body);
}

bool ParseRequest::is_correct_header(string header) {
    for (int i = 0; i < COUNT_KEY_HEADERS; i++) {
        if (_headers[i] == header) {
            return (true);
        }
    }
    return (false);
}

bool ParseRequest::is_correct_type_request(string type_request) {
    for (int i = 0; i < COUNT_TYPE_REQUESTS; i++) {
        if (_type_requests[i] == type_request) {
            return (true);
        }
    }
    return (false);
}
