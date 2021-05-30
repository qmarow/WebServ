#include "Response.hpp"

Response::Response(): _response(std::string()), _body_message(std::string()),  _request(Request()),
_code_status(int()), _url(string()), _expansion(string()){
}

Response::~Response() {}

Response::Response(Response const &x) {
    *this = x;
}

Response    &Response::operator=(const Response &x) {
    _response = x._response;
    _body_message = x._body_message;
    _request = x._request;
    _code_status = x._code_status;
    _url = x._url;
    _expansion = x._expansion;
    return (*this);
}

std::string Response::get_response() {
    // create_header();
    return (create_header());
}

void        Response::set_body_message(std::string const &body_message) {
    _body_message = body_message;
}

void        Response::set_code_status(int code_status) {
   _code_status = code_status;
}

void        Response::set_request(const Request &request) {
    _request = request;
}

void        Response::set_url(const std::string &url) {
    _url = url;
}

void        Response::set_url_redirect(const string &redirect) {
    _redirect = redirect;
}

void        Response::set_expansion(const std::string &expansion) {
    _expansion = expansion;
}

string      Response::header_data() {
    return ("Data: " + get_time() + "\n");
}


string      Response::header_server() {
    return (string("Server: WebServ") + "\n");
}

string      Response::header_last_modified(string name_file) {
    return (get_time_last_modified(name_file.c_str()) + "\n");
}

string      Response::header_content_lenght() {
    return ("Content-Lenght: " + std::to_string(_body_message.size()) + "\n");
}

string      Response::header_content_type() {
    Content_type    type;
    string          result = "Content-Type: ";
    std::vector<string> methodes_requeste = _request.get_values_header("Accept-Charset");

    if (_expansion != string()) {
        result = result + type.get_type(_expansion);
    }
    if (methodes_requeste.empty()) {
        result += "; charset=";
        result += methodes_requeste[0];
    }
    return (string("Content-Type: ") + "\n");
}

string      Response::header_content_language() {
    std::vector<string> tmp = _request.get_values_header("Accept-Language");
    string  res;
    if (!tmp.empty()) {
        res = "Content-Language: ";
        for (int i = 0; i < tmp.size(); i++) {
            res = res + tmp[i];
        }
    }
    return (res + "\n");
}

string      Response::header_allow() {
    std::vector<string> methodes_requeste = _request.get_values_header("Allow");
    string              res = "Allow: ";
    if (!methodes_requeste.empty()) {
        int have = 0;
        std::vector<string> methodes_server = _server.get_allow_methods();
        for (int i = 0; i < methodes_requeste.size(); i++) {
            for (int a = 0; a < methodes_server.size(); a++) {
                if (methodes_server[i] == methodes_requeste[a]) {
                    have = 1;
                    res = res + methodes_server[i] + ", ";
                    break ;
                }
            }
            if (have == 0)
                std::cout << "Error 405\n";
              //Error 405 Method Not Allowed
        }
        // тогда посмотреть у сервера какие методы мы можем обрабатывать и записать в Allow,
        // если такого метода нет - выкинуть ошибку "405 Method Not Allowed"
    }
    return (res + "\n");
}

string      Response::header_content_location() {
    // _response = _response + "Content-Location: "; // указать URL-адрес ресурса
    return ("\n");
}

string      Response::header_location() {
    // _response = _response + "Location: "; //Заголовка ответа указывает URL для перенаправления на страницу. Он имеет смысл только в том случае, если его обслуживают с ответом 3xx (перенаправление) или 201(создано).
    return ("Location: " + _redirect + "\n");
}

string      Response::header_retry_after() {
    // _response = _response + "Retry-After"; // Нужен только в случае ошибки 503, 429 или 301
    return ("\n");
}

void      Response::header_for_GET() {

}

void      Response::header_for_HEAD() {

}

void      Response::header_for_POST() {

}

void      Response::header_for_PUT() {

}

void      Response::header_for_DELETE() {

}

std::string Response::create_header() {
    string response = "HTTP/1.1 " + std::to_string(_code_status) + " OK\n";

    response += header_data();
    // response += header_server();
    // response += header_last_modified("название файла");
    // response += header_content_lenght();
    // response += header_content_type();
    // response += header_content_language();
    // response += header_allow();
    // response += header_content_location();
    response += header_location();
    // response += header_retry_after();
    response += "\n\n";
    response += _body_message;
    return (response);
}
