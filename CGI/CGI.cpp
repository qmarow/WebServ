#include <sys/wait.h>
#include "CGI.hpp"

CGI::CGI() {
    _env = NULL;
    _argv = NULL;
    _url = URL();
    _server = Server();
    _request = Request();
    _expansion = string();
    _url_cgi_file = string();
    _client = Client();
}

CGI::~CGI() {
    int i = 0;
    for (; *(_env + i) != 0; i++) {
        free(*(_env + i));
    }
    free(*(_env + i));
    free(_env);
}

CGI::CGI(CGI const &x) {
    *this = x;
}

CGI     &CGI::operator=(const CGI &x) {
    _env = x._env;
    _argv = x._argv;
    _url = x._url;
    _server = x._server;
    _request = x._request;
    _expansion = x._expansion;
    _url_cgi_file = x._url_cgi_file;
    _client = x._client;
    return (*this);
}

void    CGI::set_url(URL url) {
    _url = url;
}

void    CGI::set_server(const Server &x) {
    _server = x;
}

void    CGI::set_request(const Request &x) {
    _request = x;
}

void    CGI::set_expansion(string expansion) {
    _expansion = expansion;
}

char    **CGI::add_and_relocation(char **base, char *s_add) {
    int     size = 1;
    for (int i = 0; base + i != 0; i++) {
        size++;
    }
    char **result = (char**)malloc(sizeof(char*) * (size + 1));
    result[size] = 0;
    int i = 0;
    for (; *(base + i) != 0; i++) {
        result[i] = (char*)malloc(sizeof(char) * (ft_strlen(base[i]) + 1));
        int a = 0;
        for (; base[i][a] != 0; a++) {
            result[i][a] = base[i][a];
        }
        result[i][a] = 0;
    }
    int a = 0;
    for (; s_add[a] != 0; a++) {
        result[i][a] = s_add[a];
    }
    result[i][a] = 0;
    for (i = 0; *(base + i) != 0; i++) {
        free(*(base + i));
    }
    free(*(base + i));
    free(s_add);
    return (result);
}

char    *CGI::start_cgi() {
    int pipes[2];
    char *argv[2] = {const_cast<char*>(_filename_script.c_str()), 0};
    char *cgi_file =  const_cast<char*>(_server.get_cgi_pass().c_str());
    set_env();
    pid_t pid;
//    File    buf_file;

//    buf_file.open("./CGI", "BUFFER.txt");

    if ((pipe(pipes)) <= 0)
        std::cout << "Error: pipe\n";
    if ((dup2(pipes[0], 0) < 0) || (dup2(pipes[1], 1) < 0)) {
        std::cout << "Error: dup2";
    }
    if (_request.get_method() == "POST") {
        std::cout << _request.get_body();
    }
    _env = add_and_relocation(_env, const_cast<char*>(("SCRIPT_FILENAME=" + _filename_script).c_str()));
    if ((pid = fork()) < 0) {
        std::cout << "Error: fork\n";
        close(pipes[0]);
        close(pipes[1]);
        return NULL;
    } else if (pid == 0) {
        if (execve(cgi_file, argv, _env) < 0) {
            close(pipes[0]);
            close(pipes[1]);
            return NULL;
        }
    }
    int status;
    waitpid(pid, &status, 0);

    string  res = File::read_file(pipes[0]);
    close(pipes[0]);
    close(pipes[1]);
    return (const_cast<char*>(res.c_str()));
}

void    CGI::set_filename_script(char *filename_script) {
    _filename_script = filename_script;
}

void    CGI::set_env() {
    int     size = 1;
    char    *tmp;
    if ((tmp = definition_auth_type()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_content_length()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_content_type()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_gateway_interface()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_path_info()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_path_translated()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_query_string()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_remote_addr()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_remote_ident()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_remote_user()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_request_method()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_request_uri()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_script_name()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_server_name()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_server_port()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_server_protocol()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
    if ((tmp = definition_server_software()) != NULL) {
        _env = add_and_relocation(_env, tmp);
    }
}

void    CGI::set_url_cgi_file(string cgi_file) {
    _url_cgi_file = cgi_file;
}

char    *CGI::copy_string_to_char(string s1, char *s2) {
    int i = 0;
    for (; i < s1.size(); i++) {
        s2[i] = s1[i];
    }
    s2[i] = 0;
    return (s2);
}

char    *CGI::definition_auth_type() {
    vector_string auth_basic = _server.get_auth_basic();
    if (auth_basic.empty()) {
        return (NULL);
    } // если есть auth_basic тогда определяем
    string  tmp = auth_basic[0];
    char    *result;

    result = (char*)malloc(sizeof(char) * (tmp.size() + 1));
    result = copy_string_to_char(tmp, result);
    return (result);
}

char    *CGI::definition_content_length() {
    if (_request.get_method() != "POST") {
        return (NULL);
    }
    string body = _request.get_body();
    string tmp = "CONTENT_LENGHT=" + std::to_string(body.size());
    char *result = (char*)malloc(sizeof(char) * (tmp.size() + 1));
    result = copy_string_to_char(tmp, result);
    return (result);

}

char    *CGI::definition_content_type() {
    // тип содержимого посланного серверу клиентом
    if (_expansion.empty())
        return NULL;
    Content_type a;
    string  tmp = "CONTENT_TYPE=" + a.get_type(_expansion);
    char    *content_type = (char*)malloc(sizeof(char) * (tmp.size() + 1));
    content_type = copy_string_to_char(tmp, content_type);

    return (content_type);
}

char    *CGI::definition_gateway_interface() {
//Указывает версию интерфейса CGI, который поддерживает сервер.
    char    *version_CGI;

    version_CGI = (char*)malloc(sizeof(char) * (7 + 1)); //CGI/1.1
    version_CGI = copy_string_to_char("GATEWAY_INTERFACE=CGI/1.1", version_CGI);
    return (version_CGI);
}

char    *CGI::definition_path_info() {
// относильный путь к исполняемому cgi ресурсу? пока точно не знаю
}

char    *CGI::definition_path_translated() {
    // абсолютный путь к исполняемомоу cgi ресурсу или скрипту? пока точно не знаю
}

char    *CGI::definition_query_string() {
    // переменные URL
    if (_url.get_values_argv().empty() || _request.get_method() != "GET")
        return (NULL);
    vector_string   keys = _url.get_keys_argv();
    vector_string   value = _url.get_values_argv();
    string          tmp = "QUERY_STRING=" + keys[0] + "=" + value[0];;
    char            *result = NULL;

    int i = 1;
    for (; i < keys.size(); i++) {
        tmp = tmp + "&" + keys[i] + "=" + value[i];
    }

    result = (char*)malloc(sizeof(char) * (tmp.size() + 1));
    result = copy_string_to_char(tmp, result);
    return (result);
}

char    *CGI::definition_remote_addr() {
    // IP клиета
    struct sockaddr_in data_socket = _client.get_data_socket();
    string tmp  = "REMOTE_ADDR=" + string(inet_ntoa(data_socket.sin_addr));
    char    *result;

    result = (char*)(malloc(sizeof(char) * (tmp.size() + 1)));
    result = copy_string_to_char(tmp, result);
    return (result);
}

char    *CGI::definition_remote_ident() {
//    Напрмер, если имя удаленного пользователя pschmauder и
//    он назодится на удаленном узле jamsa.com ,
//    то переменная примет следующее значение:
//    REMOTE_IDENT = pschmauder.www.jamsa.com
}

char    *CGI::definition_remote_user() {
//    Используется для того, чтобы получить имя удаленного пользователя без имени узла
}

char    *CGI::definition_request_method() {
    // определяет тип запроса GET, HEAD POST и т.д.
    string  tmp = "REQUEST_METHOD=" + _request.get_method();
    char    *result;

    result = (char*)malloc(sizeof(char) * (tmp.size() + 1));
    result = copy_string_to_char(tmp, result);
    return (result);
}

char    *CGI::definition_request_uri() {
    vector_string request_uri = _request.get_values_header("Content-Location");
    if (request_uri.empty())
        return (NULL);
    string  tmp = "REQUEST_URI=" + request_uri[0];
    char    *result;

    result = (char*)malloc(sizeof(char) * (tmp.size() + 1));
    result = copy_string_to_char(tmp, result);
    return (result);
}

char    *CGI::definition_script_name() {
    if (_url_cgi_file.empty())
        return (NULL);

    char    *result;

    result = (char*)malloc(sizeof(char) * (_url_cgi_file.size() + 1));
    result = copy_string_to_char("SCRIPT_NAME=" + _url_cgi_file, result);
    return (result);
}

char    *CGI::definition_server_name() {
    string  tmp = _server.get_server_name();
    char    *result;

    result = (char*)malloc(sizeof(char) * (tmp.size() + 1));
    result = copy_string_to_char("SERVER_NAME=" + tmp, result);
    return (result);
}

char    *CGI::definition_server_port() {
    string  tmp = std::to_string(_server.get_port());
    char    *result;

    result = (char*)malloc(sizeof(char) * (tmp.size() + 1));
    result = copy_string_to_char("SERVER_PORT=" + tmp, result);
    return (result);
}

char    *CGI::definition_server_protocol() {
    char    *result;

    result = (char*)malloc(sizeof(char) * (8 + 1));
    result = copy_string_to_char("SERVER_PROTOCOL=HTTP/1.1", result);
    return (result);
}

char    *CGI::definition_server_software() {
    char    *result;

    result = (char*)malloc(sizeof(char) * (11 + 1));
    result = copy_string_to_char("SERVER_SOFTWARE=WebServ/1.1", result);
    return (result);
}
