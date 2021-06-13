#ifndef PARSECONFIGE_HPP
#define PARSECONFIGE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include "Server.hpp"
#include "./../utils/UtilsString.hpp"

#define COUNT_KEY_WORDS 22
#define COUNT_TYPE_REQUESTS 5

class ParseConfige {
public:
    typedef std::string                         string;
    typedef std::vector<string>                 vector_string;
    typedef std::vector<int>                    vector_int;
    typedef std::map<string, vector_string >    map_directives;
    typedef std::map<string, Server>            map_locations;
    typedef std::vector<Server>                 vector_server;

private:
    static const char *_key_words[COUNT_KEY_WORDS];
    static const char *_type_requests[COUNT_TYPE_REQUESTS];
    vector_server  _servers;

public:
    ParseConfige();
    ParseConfige(const ParseConfige &parse_data_server);
    ParseConfige &operator=(const ParseConfige &parse_data_server);
    ~ParseConfige();

    void            open(string name_file);
    vector_server   get_servers(void);

    class UnknownParamsException: public std::exception {
    public:
        const char* what() const throw();
	};

	class BadConfgFileException: public std::exception {
    public:
        const char* what() const throw();
	};

	class NeedOpenParthenesException: public std::exception {
    public:
        const char* what() const throw();
	};

    class NeedSemicolonException: public std::exception {
    public:
        const char* what() const throw();
    };

    class DirectiveIncorrectlyException: public std::exception {
    public:
        const char *what() const throw();
    };

    class IdenticalsPortsException: public std::exception {
    public:
        const char *what() const throw();
    };

private:
    int             check_valid_port_and_name(void);
    void            add_locations(Server &location, int &count_parentheses, std::ifstream &confirugation);
    void            add_directive(Server &server, vector_string words);
    int             get_words(string line, vector_string &words);
    bool            is_correct_word(string word);
    bool            is_correct_method(string word);
    int             find_parentheses(vector_string words);
    int             find_semicolon(vector_string words);

    string          parse_ip(vector_string values);
    int             parse_port(vector_string values);
    int             parse_max_body_size(vector_string values);
    string          parse_root(vector_string values);
    string          parse_server_name(vector_string values);
    vector_string   parse_index(vector_string values);
    bool            parse_autoindex(vector_string values);
    vector_int      parse_cods_error_page(vector_string values);
    string          parse_file_error_page(vector_string values);
    int             parse_code_redirect(vector_string values);
    string          parse_url_redirect(vector_string values);
    vector_string   parse_methods(vector_string values);
    string          parse_cgi_pass(vector_string values);
    string          parse_cgi_param(vector_string values);
};

#endif