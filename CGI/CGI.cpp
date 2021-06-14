#include "CGI.hpp"
#include "./../utils/UtilsPrint.ipp"

CGI::CGI() {
    _env = vector_string();
    _argv = vector_string();
    _url = URL();
    _server = Server();
    _request = Request();
    _expansion = string();
    _url_cgi_file = string();
}

CGI::~CGI() {}

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

void    CGI::set_filename_script(string filename_script) {
    _filename_script = filename_script;
}

void    CGI::set_url_cgi_file(string cgi_file) {
    _url_cgi_file = cgi_file;
}

void    CGI::set_data_socket_client(sockaddr_t data_socket_client) {
    _data_socket_client = data_socket_client;
}

// string  CGI::start() {
//     std::cout << "\n\tCGI\n";
//     int             status;
//     pid_t           pid;
//     // string          result;
//     string          cgi_exec;
//     string          cgi_script;
//     vector_string   argv;
//     char            **argv_c;
//     char            **env_c;
//     int             fd_write;
//     int             fd_file;

//     set_env();
//     cgi_exec = "/Users/qmarowak/Desktop/WebServ/" + trim_line(_server.get_cgi_pass(), "./");
//     // cgi_script = "/Users/utoomey/Desktop/WebServ/" + trim_line(_filename_script, "./");


//     cgi_script = "/Users/qmarowak/Desktop/WebServ/cgi_tester";

//     std::cout << "cgi_exec - " << cgi_exec << "\n";
//     std::cout << "cgi_script - " << cgi_script << "\n";
//     argv.push_back(cgi_exec);
//     argv.push_back(cgi_script);
//     // if ((pipe(pipes)) <= 0) {
//     //     std::cout << "Error: pipe\n";
//     // }
//     _env.push_back("SCRIPT_FILENAME=" + cgi_script);
//     argv_c = convert_array_string_to_char(argv);
//     env_c = convert_array_string_to_char(_env);

//     // std::cout << "ENV - " << env_c << "\n";
//     for (int i = 0; env_c[i] != NULL; i++) {
//         std::cout << "str " << i << ": ";
//         // for (int a = 0; env_c[i][a] != 0; i++) {
//             std::cout << env_c[i] << "\n";
//     }
//     fd_write = dup(1);
//     int fd = open("./youpi.bla", O_RDONLY);
//     fd_file = open("./tmp.txt", O_CREAT | O_WRONLY | O_TRUNC, ~0);
//     if (fd_file < 0) {
//         std::cout << "fd_file err\n";
//         exit(1);
//     }
//     if ((pid = fork()) < 0) {
//         print_error("Error: fork");
//     } else if (pid == 0) {

//         if (dup2(fd, 0) < 0) {
//             print_error("Error: dup2");
//         }
//         if (dup2(fd_file, 1) < 0) {
//             print_error("Error: dup2");
//         }
//         if (execve(cgi_exec.c_str(), argv_c, NULL) < 0) {
//             print_error("Error: dup2");
//         }
//     }
//     std::cout << "Waitpid...\n";
//     waitpid(pid, &status, 0);
//     // std::cout << " \\/ Waitpid\n";
//     close(fd_file);
//     close(fd);
//     free_array_char(argv_c, argv.size());
//     free_array_char(env_c, _env.size());
//     fd_file = open("./tmp.txt", O_RDONLY);

//     string res = File::read_file(fd_file);

//     // std::cout << "\n|" << res << "|\n";
//     return (res);
// }

string  CGI::start() {
    // int             pipes[2];
    int             status;
    pid_t           pid;
    // string          result;
    string          cgi_exec;
    string          cgi_script;
    vector_string   argv;
    char            **argv_c;
    char            **env_c;
    int             fd_write;
    int             fd_file;

    set_env();
    cgi_exec = "/Users/utoomey/Desktop/WebServ/" + trim_line(_server.get_cgi_pass(), "./");
    cgi_script = "/Users/utoomey/Desktop/WebServ/" + trim_line(_filename_script, "./");
    argv.push_back(cgi_exec);
    argv.push_back(cgi_script);
    // if ((pipe(pipes)) <= 0) {
    //     std::cout << "Error: pipe\n";
    // }
    _env.push_back("SCRIPT_FILENAME=" + cgi_script);
    argv_c = convert_array_string_to_char(argv);
    env_c = convert_array_string_to_char(_env);
    fd_write = dup(1);
    fd_file = open("./other/tmp/tmp.txt", O_CREAT | O_WRONLY | O_TRUNC, ~0);
    if ((pid = fork()) < 0) {
        print_error("Error: fork");
    } else if (pid == 0) {
        if (dup2(fd_file, 1) < 0) {
            print_error("Error: dup2");
        }
        if (execve(cgi_exec.c_str(), argv_c, NULL)) {
            print_error("Error execve");
        }
    }
    waitpid(pid, &status, 0);
    close(fd_file);
    free_array_char(argv_c, argv.size());
    free_array_char(env_c, _env.size());
    fd_file = open("./other/tmp/tmp.txt", O_RDONLY);
    return (File::read_file(fd_file));
}

char **CGI::convert_array_string_to_char(vector_string array_string) {
    char **array_char;

    array_char = (char**)malloc(sizeof(char*) * (array_string.size() + 1));
    for (int i = 0; i < array_string.size(); i++) {
        array_char[i] = strdup(array_string[i].c_str());
    }
    array_char[array_string.size()] = NULL;
    return (array_char);
}

void    CGI::free_array_char(char **array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

void    CGI::set_env() {
    string  value;

    if ((value = definition_auth_type()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_content_length()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_content_type()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_gateway_interface()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_path_info()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_path_translated()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_query_string()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_remote_addr()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_remote_ident()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_remote_user()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_request_method()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_request_uri()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_script_name()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_server_name()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_server_port()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_server_protocol()) != "") {
        _env.push_back(value);
    }
    if ((value = definition_server_software()) != "") {
        _env.push_back(value);
    }
}

string  CGI::definition_auth_type() {
    vector_string auth_basic;
    
    auth_basic = _server.get_auth_basic();
    if (auth_basic.empty()) {
        return ("");
    } // если есть auth_basic тогда определяем
    return (auth_basic[0]);
}

string  CGI::definition_content_length() {
    string body;
    string content_length;
 
    if (_request.get_method() != "POST") {
        return ("");
    }
    body = _request.get_body();
    content_length = "CONTENT_LENGHT=" + std::to_string(body.size());
    return (content_length);

}

string  CGI::definition_content_type() {
    string content_type;

    // тип содержимого посланного серверу клиентом
    if (_expansion.empty())
        return ("");
    content_type = "ContentType=" + ContentType().get_type(_expansion);
    return (content_type);
}

string  CGI::definition_gateway_interface() {
    //Указывает версию интерфейса CGI, который поддерживает сервер.
    return (string("GATEWAY_INTERFACE=CGI/1.1"));
}

string CGI::definition_path_info() {
// относильный путь к исполняемому cgi ресурсу? пока точно не знаю
    return ("");
}

string CGI::definition_path_translated() {
    // абсолютный путь к исполняемомоу cgi ресурсу или скрипту? пока точно не знаю
    return ("");
}

string CGI::definition_query_string() {
    vector_string   keys;
    vector_string   value;
    string          result;

    // переменные URL
    if (_url.get_values_argv().empty() || _request.get_method() != "GET") {
        return ("");
    }
    keys = _url.get_keys_argv();
    value = _url.get_values_argv();
    result = "QUERY_STRING=" + keys[0] + "=" + value[0];;
    for (int i = 1; i < keys.size(); i++) {
        result = result + "&" + keys[i] + "=" + value[i];
    }
    return (result);
}

string CGI::definition_remote_addr() {
    // IP клиета
    return ("REMOTE_ADDR=" + string(inet_ntoa(_data_socket_client.sin_addr)));
}

string CGI::definition_remote_ident() {
//    Напрмер, если имя удаленного пользователя pschmauder и
//    он назодится на удаленном узле jamsa.com ,
//    то переменная примет следующее значение:
//    REMOTE_IDENT = pschmauder.www.jamsa.com
    return ("");
}

string CGI::definition_remote_user() {
//    Используется для того, чтобы получить имя удаленного пользователя без имени узла
    return ("");
}

string CGI::definition_request_method() {
    // определяет тип запроса GET, HEAD POST и т.д.
    return ("REQUEST_METHOD=" + _request.get_method());
}

string CGI::definition_request_uri() {
    vector_string   request_uri;
    
    if (request_uri.empty()) {
        return ("");
    }
    request_uri = _request.get_values_header("Content-Location");
    return ("REQUEST_URI=" + request_uri[0]);
}

string CGI::definition_script_name() {
    if (_url_cgi_file.empty())
        return ("");
    return ("SCRIPT_NAME=" + _url_cgi_file);
}

string CGI::definition_server_name() {
    return (_server.get_server_name());
}

string CGI::definition_server_port() {
    return ("SERVER_PORT=" + std::to_string(_server.get_port()));
}

string CGI::definition_server_protocol() {
    return ("SERVER_PROTOCOL=HTTP/1.1");
}

string CGI::definition_server_software() {
    return ("SERVER_SOFTWARE=WebServ/1.1");
}
