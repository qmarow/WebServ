#include "ParseConfige.hpp"

// PRIVATE

const char *ParseConfige::_key_words[COUNT_KEY_WORDS] = {"client_max_body_size", "root", "index", "autoindex", "error_page", 
                                            "location", "cgi_pass", "cgi_param" , "return", "auth_basic",
                                            "auth_basic_user_file", "allow_methods", "deny_methods", "server_name",
                                            "cgi_allow_methods", "cgi_deny_methods", "default_method_body",
                                            "unknown", "server", "listen", "}", "{"};

const char *ParseConfige::_type_requests[COUNT_TYPE_REQUESTS] = {"GET", "HEAD", "POST", "PUT", "DELETE"};

// PUBLIC
ParseConfige::ParseConfige() {
    _servers = vector_server();
}

ParseConfige::ParseConfige(const ParseConfige &parse_data_server) {
    *this = parse_data_server;
}

ParseConfige &ParseConfige::operator=(const ParseConfige &parse_data_server) {
    _servers = parse_data_server._servers;
    return (*this);
}

ParseConfige::~ParseConfige() {}

std::vector<Server> ParseConfige::get_servers(void) {
    return (_servers);
}

void ParseConfige::open(std::string name_file) {
    std::ifstream           configuration(name_file.size() > 0 ? name_file : "webserv.conf");
    vector_string           words;
    string                  line;
    int                     flag_server;
    int                     count_parentheses = 0;

    if (!configuration.is_open()) {
        throw BadConfgFileException();
    }
    while (std::getline(configuration, line, '\n')) {
        if (get_words(line, words) == 0) {
            continue ;
        }
        count_parentheses += find_parentheses(words);
        if (words[0] == "server") {
            _servers.push_back(Server());
        } else if (words[0] == "location") {
            Server &last_server = _servers[_servers.size() - 1];
            Server location;
            location.copy_directive(last_server);
            add_locations(location, count_parentheses, configuration);
            last_server.set_location(words[1], location);
        } else if (words[0] != "{" && words[0] != "}") {
            add_directive(_servers[_servers.size() - 1], words);
        }
    }
    if (count_parentheses != 0) {
        throw NeedOpenParthenesException();
    }
    if (check_valid_port_and_name()) {
        throw IdenticalsPortsException();
    }
}

int ParseConfige::check_valid_port_and_name(void) {
    for (int i = 0; i < _servers.size(); i++) {
        for (int k = 0; k < _servers.size(); k++) {
            if (_servers[i].get_port() == _servers[k].get_port() && i != k) {
                return (1);
            }
            if (_servers[i].get_server_name() == _servers[k].get_server_name() && i != k) {
                return (1);
            }
        }
    }
    return (0);
}

// PRIVATE

int ParseConfige::get_words(string line, vector_string &words) {
    words.clear();
    line = erase_after_symbol(line, '#');
    line = trim_line(line, " \t");
    if (line.size() == 0) {
        return (0);
    }
    line = overlay_symbol(line, '}', ' ');
    line = overlay_symbol(line, ';', ' ');
    words = split_line(line, " \t");
    if (words.size() == 0) {
        return (0);
    }
    if (is_correct_word(words[0]) == false) {
        throw UnknownParamsException();
    }
    return (1);
}

void ParseConfige::add_locations(Server &location,
                    int &count_parentheses, std::ifstream &configuration) {
    string          line;
    vector_string    words;

    while (std::getline(configuration, line, '\n')) {
        if (get_words(line, words) == 0) {
            continue ;
        }
        count_parentheses += find_parentheses(words);
        if (words[0] == "location") {
            Server new_location;
            new_location.copy_directive(location);
            add_locations(new_location, count_parentheses, configuration);
            location.set_location(words[1], new_location);
        } else {
            if (find_parentheses(words) == -1) {
                break;
            }
            if (words[words.size() - 1] != ";") {
                throw NeedSemicolonException();
            }
            add_directive(location, words);
        }
    }
}

void ParseConfige::add_directive(Server &server, vector_string words) {
    if (words[words.size() - 1] != ";") {
        throw NeedSemicolonException();
    }
    vector_string values(words.begin() + 1, words.end() - find_semicolon(words));
    if (words[0] == "client_max_body_size") {
        server.set_max_body_size(this->parse_max_body_size(values));
    }
    if (words[0] == "server_name") {
        server.set_server_name(this->parse_server_name(values));
    }
    if (words[0] == "listen") {
        server.set_ip(this->parse_ip(values));
        server.set_port(this->parse_port(values));
    }
    if (words[0] == "root") {
        server.set_root(this->parse_root(values));
    }
    if (words[0] == "index") {
        server.set_index(this->parse_index(values));
    }
    if (words[0] == "autoindex") {
        server.set_autoindex(this->parse_autoindex(values));
    }
    if (words[0] == "error_page") {
        vector_int cods = this->parse_cods_error_page(values);
        string file = this->parse_file_error_page(values);
        server.set_error_page(cods, file);
    }
    if (words[0] == "cgi_pass") {
        server.set_cgi_pass(this->parse_cgi_pass(values));
    }
    if (words[0] == "cgi_param") {
        server.set_cgi_param(this->parse_cgi_param(values));
    }
    if (words[0] == "return") {
        int code = this->parse_code_redirect(values);
        string url = this->parse_url_redirect(values);
        server.set_redirect(code, url);
    }
    if (words[0] == "auth_basic") {
        // server
    }
    if (words[0] == "auth_basic_user_file") {

    }
    if (words[0] == "allow_methods") {
        server.set_allow_methods(this->parse_methods(values));
    }
    if (words[0] == "deny_methods") {
        server.set_deny_methods(this->parse_methods(values));
    }
    if (words[0] == "cgi_allow_methods") {
        server.set_cgi_allow_methods(this->parse_methods(values));
    }
    if (words[0] == "cgi_deny_methods") {
        server.set_cgi_deny_methods(this->parse_methods(values));
    }
}

bool ParseConfige::is_correct_word(string word) {
    for (int i = 0; i < COUNT_KEY_WORDS; i++) {
        if (word == _key_words[i]) {
            return (true);
        }
    }
    return (false);
}

bool ParseConfige::is_correct_method(string method) {
    for (int i = 0; i < COUNT_TYPE_REQUESTS; i++) {
        if (method == _type_requests[i]) {
            return (true);
        }
    }
    return (false);
}

int ParseConfige::find_parentheses(vector_string words) {
    int count = 0;

    for (int i = 0; i < words.size(); i++) {
        if (words[i] == "}") {
            count--;
        } else if (words[i] == "{") {
            count++;
        }
    }
    return (count);
}

int ParseConfige::find_semicolon(vector_string words) {
    int end = 1;
    for (int i = 1; i < words.size(); i++) {
        if (*(words.end() - i) == ";") {
            end = i;
        }
    }
    return (end);
}

string          ParseConfige::parse_ip(vector_string values) {
    if (values.size() != 1) {
        throw DirectiveIncorrectlyException();
    }
    string value = values[0];
    if (value.find(":") == std::string::npos) {
        throw DirectiveIncorrectlyException();
    }
    values = split_line(value, ":");
    if (values.size() != 2) {
        throw DirectiveIncorrectlyException();
    }
    if (split_line(values[0], ".").size() != 4) {
        throw DirectiveIncorrectlyException();
    }
    return (values[0]);
}

int             ParseConfige::parse_port(vector_string values) {
    if (values.size() != 1) {
        throw DirectiveIncorrectlyException();
    }
    string value = values[0];
    if (value.find(":") == std::string::npos) {
        throw DirectiveIncorrectlyException();
    }
    values = split_line(value, ":");
    if (values.size() != 2) {
        throw DirectiveIncorrectlyException();
    }
    int port = std::atoi(values[1].c_str());
    if (port < 0) {
        throw DirectiveIncorrectlyException();
    }
    return (port);
}

int             ParseConfige::parse_max_body_size(vector_string values) {
    if (values.size() != 1) {
        throw DirectiveIncorrectlyException();
    }
    int value = std::atoi(values[0].c_str());
    if (value < 0) {
        throw DirectiveIncorrectlyException();
    }
    return (value);
}

string          ParseConfige::parse_root(vector_string values) {
    if (values.size() != 1) {
        throw DirectiveIncorrectlyException();
    }
    return (values[0]);
}

string          ParseConfige::parse_server_name(vector_string values) {
        if (values.size() != 1) {
        throw DirectiveIncorrectlyException();
    }
    return (values[0]);
}

std::vector<std::string>    ParseConfige::parse_index(vector_string values) {
    if (values.size() < 1) {
        throw DirectiveIncorrectlyException();
    }
    return (values);
}

bool            ParseConfige::parse_autoindex(vector_string values) {
    if (values.size() != 1) {
        throw DirectiveIncorrectlyException();
    }
    if (values[0] != "off" && values[0] != "on") {
        throw DirectiveIncorrectlyException();
    }
    return (values[0] == "on");
}

std::vector<int>  ParseConfige::parse_cods_error_page(vector_string values) {
    if (values.size() < 2) {
        throw DirectiveIncorrectlyException();
    }
    vector_int cods;
    for (int i = 0; i < values.size() - 1; i++) {
        int value = std::atoi(values[i].c_str());
        if (value < 400 || value > 505) {
            throw DirectiveIncorrectlyException();
        }
        cods.push_back(value);
    }
    return (cods);
}

string          ParseConfige::parse_file_error_page(vector_string values) {
    if (values.size() < 2) {
        throw DirectiveIncorrectlyException();
    }
    return (values[values.size() - 1]);
}

int             ParseConfige::parse_code_redirect(vector_string values) {
    if (values.size() != 2) {
        throw DirectiveIncorrectlyException();
    }
    int value = std::atoi(values[0].c_str());
    if (value > 307 || value < 300) {
        throw DirectiveIncorrectlyException();
    }
    return (value);
}

string          ParseConfige::parse_url_redirect(vector_string values) {
    if (values.size() != 2) {
        throw DirectiveIncorrectlyException();
    }
    return (values[1]);
}

vector_string   ParseConfige::parse_methods(vector_string values) {
    if (values.size() == 0) {
        throw DirectiveIncorrectlyException();
    }
    for (int i = 0; i < values.size(); i++) {
        if (is_correct_method(values[i]) == false) {
            throw DirectiveIncorrectlyException();
        }
    }
    return (values);
}

string          ParseConfige::parse_cgi_pass(vector_string values) {
    if (values.size() != 1) {
        throw DirectiveIncorrectlyException();
    }
    return (values[0]);
}

string          ParseConfige::parse_cgi_param(vector_string values) {
    if (values.size() != 1) {
        throw DirectiveIncorrectlyException();
    }
    return (values[0]);
}



// PUBLIC

const char *ParseConfige::UnknownParamsException::what() const throw() {
    return ("Unknown parametrs in configurtion file!");
}

const char *ParseConfige::BadConfgFileException::what() const throw() {
    return ("Bad configuration file!");
}

const char *ParseConfige::NeedOpenParthenesException::what() const throw() {
    return ("Need parentheses!");
}

const char *ParseConfige::NeedSemicolonException::what() const throw() {
    return ("Need semicolon!");
}

const char *ParseConfige::DirectiveIncorrectlyException::what() const throw() {
    return ("The directive is specified incorrectly!");
}

const char *ParseConfige::IdenticalsPortsException::what() const throw() {
    return ("The configuration has the same ports!");
}