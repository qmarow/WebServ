#include "Server.hpp"

// PUBLIC

Server::Server() {
    _ip = string("127.0.0.1");
    _port = 8080;
    _max_body_size = -1;
    _root = string();
    _server_name = string();
    _index = vector_string();
    _autoindex = false;
    _error_pages = vector_error_page();
    _keys_locations = vector_string();
    _locations = vector_server();
    _cgi_pass = string();
    _cgi_param = string();
    _redirect = t_redirect();
    _redirect.code = -1;
    _auth_basic = vector_string ();
    //auth_basic_user_file
    _allow_methods = vector_string();
    _cgi_allow_methods = vector_string();
}

Server::Server(const Server &parse_confige) {
    *this = parse_confige;
}

Server &Server::operator=(const Server &server) {
    copy_directive(server);
    _keys_locations = server._keys_locations;
    _locations = server._locations;
    return (*this);
}

Server::~Server() {}

void Server::copy_directive(const Server &server) {
    _ip = server._ip;
    _port = server._port;
    _max_body_size = server._max_body_size;
    _root = server._root;
    _server_name = server._server_name;
    _index = server._index;
    _autoindex = server._autoindex;
    _error_pages = server._error_pages;
    _cgi_pass = server._cgi_pass;
    _cgi_param = server._cgi_param;
    _redirect = server._redirect;
    _auth_basic = server._auth_basic;
    //auth_basic_user_file
    _allow_methods = server._allow_methods;
    _cgi_allow_methods = server._cgi_allow_methods;
    //default_method_body
}

std::string Server::get_ip(void) {
    return (_ip);
}

int Server::get_port(void) {
    return (_port);
}

int Server::get_max_body_size(void) {
    return (_max_body_size);
}

string Server::get_root(void) {
    return (_root);
}

std::string Server::get_server_name(void) {
    return (_server_name);
}

std::vector<std::string> Server::get_index(void) {
    return (_index);
}

std::string Server::get_file_error_page(int code_error) {
    for (int i = 0; i < _error_pages.size(); i++) {
        if (_error_pages[i].code == code_error) {
            return (_error_pages[i].file);
        }
    }
    return ("");
}

std::vector<int> Server::get_cods_error_page(void) {
    vector_int cods;

    for (int i = 0; i < _error_pages.size(); i++) {
        cods.push_back(_error_pages[i].code);
    }
    return (cods);
}

std::vector<t_error_page>   Server::get_error_pages(void) {
    return (_error_pages);
}

Server &Server::get_location(string key_location) {
    int index = find_word(_keys_locations, key_location);
    return (_locations[index]);
}

Server &Server::get_location(int index) {
    if (index >= _locations.size()) {
        // return (Server());
    }
    return (_locations[index]);
}

std::vector<std::string> &Server::get_keys_locations(void) {
    return (_keys_locations);
}

int Server::get_code_redirect(void) {
    return (_redirect.code);
}

std::string Server::get_url_redirect(void) {
    return (_redirect.url);
}

std::vector<std::string> Server::get_auth_basic(void) {
    return (_auth_basic);
}

std::vector<std::string> Server::get_allow_methods(void) {
    return (_allow_methods);
}

std::vector<std::string> Server::get_cgi_allow_methods(void) {
    return (_cgi_allow_methods);
}

std::string Server::get_cgi_param(void) {
    return (_cgi_param);
}

std::string     Server::get_cgi_pass() {
    return (_cgi_pass);
}

void Server::set_ip(string ip) {
    _ip = ip;
}

void Server::set_port(int port) {
    _port = port;
}

void Server::set_max_body_size(int max_body_size) {
    _max_body_size = max_body_size;
}

void Server::set_root(string root) {
    _root = root;
}

void  Server::set_server_name(string server_name) {
    _server_name = server_name;
}

void  Server::set_index(vector_string index) {
    _index = index;
}

void Server::set_autoindex(bool autoindex) {
    _autoindex = autoindex;
}

void  Server::set_error_page(vector_int cods, string file) {
    for (int i = 0; i < cods.size(); i++) {
        t_error_page error_page;

        error_page.code = cods[i];
        error_page.file = file;
        _error_pages.push_back(error_page);
    }
}

void  Server::set_location(string url, Server &location) {
    url = trim_line(url, "/");
    _keys_locations.push_back(url);
    _locations.push_back(location);
}

void  Server::set_redirect(int code, string url) {
    _redirect.code = code;
    _redirect.url = url;
}

void Server::set_auth_basic(vector_string auth_basic) {
    _auth_basic = auth_basic;
}



void  Server::set_allow_methods(vector_string methods) {
    // print_vector("!!! ", methods);
    for (int i = 0; i < methods.size(); i++) {
        if (find_word(_allow_methods, methods[i]) == -1) {
            _allow_methods.push_back(methods[i]);
        }
    }
    
}

void  Server::set_deny_methods(vector_string methods) {
    int index;
    vector_string::iterator it;

    for (int i = 0; i < methods.size(); i++) {
        index = find_word(_allow_methods, methods[i]);
        if (index != -1) {
            it = _allow_methods.begin();
            _allow_methods.erase(it + index);
        }
    }
}

void  Server::set_cgi_allow_methods(vector_string methods) {
    for (int i = 0; i < methods.size(); i++) {
        if (find_word(_cgi_allow_methods, methods[i]) == -1) {
            _cgi_allow_methods.push_back(methods[i]);
        }
    }
}

void  Server::set_cgi_deny_methods(vector_string methods) {
    int index;
    vector_string::iterator it;

    for (int i = 0; i < methods.size(); i++) {
        index = find_word(_cgi_allow_methods, methods[i]);
        if (index != -1) {
            it = _cgi_allow_methods.begin();
            _cgi_allow_methods.erase(it + index);
        }
    }
}

void    Server::set_cgi_pass(string cgi_pass) {
    _cgi_pass = cgi_pass;
}

void    Server::set_cgi_param(string cgi_param) {
    _cgi_param = cgi_param;
}

bool Server::is_max_body_size(void) {
    return (_max_body_size != -1);
}

bool Server::is_root(void) {
    return (_root != "");
}

bool Server::is_server_name(void) {
    return (_server_name.size());
}

bool Server::is_index(void) {
    return (_index.size());
}

bool Server::is_autoindex(void) {
    return (_autoindex);
}

bool Server::is_error_page(int code_error) {
    return (get_file_error_page(code_error) != "");
}

bool Server::is_error_page(void) {
    return (_error_pages.size() != 0);
}

bool Server::is_location(string key_location) {
    return (find_word(_keys_locations, key_location) != -1);
}

bool Server::is_method(string method) {
    return (find_word(_allow_methods, method) != -1);
}

bool Server::is_allow_methods(void) {
    return (_allow_methods.size() != 0);
}

bool Server::is_cgi_method(string method) {
    return (find_word(_cgi_allow_methods, method) != -1);
}

bool Server::is_cgi_allow_methods(void) {
    return (_cgi_allow_methods.size() != 0);
}

bool Server::is_redirect(void) {
    return (_redirect.code != -1);
}

bool Server::is_cgi_pass(void) {
    return (_cgi_pass.size() != 0);
}

bool Server::is_cgi_param(void) {
    return (_cgi_param.size() != 0);
}

bool Server::is_authorization(string body, string tmp){
    int a = body.find(tmp);

    if (a != 4) {
        return (false);
    }
    int i = 0;
    for (; i < tmp.size(); i++) {
        if (tmp[i] != body[a]) {
            return (false);
        }
        ++a;
    }
    if (tmp[i] != body[a]) {
        return (false);
    }
    return (true);
}

bool Server::is_registration(string body){
    string tmp = "    Registration";
    int i = 0;
    int a = 0;
    while (i < 4 && body[i] != 0)
        ++i;
    for (; body[i] != '\n'; i++) {
        if (body[i] != tmp[i]) {
            return (false);
        }
    }
    i += 2;
    tmp = "Login";
    for (; body[i] != ':'; i++) {
        if (body[i] != tmp[a]) {
            return (false);
        }
        ++a;
    }
    for (; body[i] != '\n'; i++) {
    }
    ++i;
    tmp = "Password";
    a = 0;
    for (; body[i] != ':'; i++) {
        if (body[i] != tmp[a]) {
            return (false);
        }
        ++a;
    }
    return (true);
}

bool            Server::is_authorization(Request request) {
    std::vector<string> tmp = request.get_values_header("Authorization");

    if (!tmp.empty()) {
        return (true);
    }
    return (false);
}
