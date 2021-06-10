#include "ErrorPage.hpp"

ErrorPage::ErrorPage() {
    _errors_page_config = std::vector<t_error_page>();
    _root_error_page = std::string();
    _code_error = int();
}

ErrorPage::~ErrorPage() {

}

ErrorPage::ErrorPage(ErrorPage const &x) {
    *this = x;
}

ErrorPage   &ErrorPage::operator=(ErrorPage const &x) {
    _errors_page_config = x._errors_page_config;
    _root_error_page = x._root_error_page;
    _code_error = x._code_error;
    return (*this);
}

int         ErrorPage::_codes_errors[COUNT_ERROR_PAGE] = {
    400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411,
    412, 413, 414, 415, 416, 417, 500, 501, 502, 503, 504, 505
};

std::string ErrorPage::_codes_names[COUNT_ERROR_PAGE] = {
        "Bad Request", "Unauthorized", "Payment Required", "Forbidden",
        "Not Found", "Method Not Allowed", "Not Acceptable",
        "Proxy Authentication Required", "Request Timeout", "Conflict",
        "Gone", "Length Required", "Precondition Failed", "Request Entity Too Large",
        "Request-URI Too Long", "Unsupported Media Type", "Requested Range Not Satisfiable",
        "Expectation Failed", "Internal Server Error",
        "Not Implemented", "Bad Gateway", "Service Unavailable",
        "Gateway Timeout", "HTTP Version Not Supported"
};

void            ErrorPage::set_code_error(int code_error) {
    _code_error = code_error;
}

void            ErrorPage::set_error_pages(std::vector<t_error_page> errors_page_config) {
    _errors_page_config = errors_page_config;
}

void            ErrorPage::set_root_error_page(std::string root) {
    _root_error_page = root;
}

std::string     ErrorPage::get_error_page(void) {
    std::string result;

    if (!(_errors_page_config.empty() || _root_error_page.empty())) {
        int     i = 0;
        File    tool_for_file;
        for (; i < _errors_page_config.size(); i++) {
            if (_errors_page_config[i].code == _code_error) {
                break ;
            }
        }
        tool_for_file.open_file(_root_error_page, _errors_page_config[i].file);
        result = tool_for_file.read_file();
        return (result);
    }
    for (int i = 0; i < COUNT_ERROR_PAGE; i++){
        if (_codes_errors[i] == _code_error) {
            result = create_error_page(_code_error, _codes_names[i]);
            break ;
        }
    }
    return (result);
}

std::string     ErrorPage::get_name_error(void) {
    for (int i = 0; i < COUNT_ERROR_PAGE; i++) {
        if (_code_error == _codes_errors[i]) {
            return (_codes_names[i]);
        }
    }
    return ("");
}

int             ErrorPage::get_code_error(void) {
    return (_code_error);
}

std::string     ErrorPage::create_error_page(int code_error, std::string name_error) {
    std::string html;

    html += "<html>\n";
    html +=     "\t<head>\n";
    html +=     "\t</head>\n";
    html +=     "\t<body>\n";
    html +=         "\t\t<h1>Error: " + std::to_string(code_error) + "</h1>\n";
    html +=             "\t\t<h2>" + name_error + "</h2>\n";
    html +=     "\t</body>\n";
    html += "</html>\n";
    return (html);
}
