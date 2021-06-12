#include "Client.hpp"

// PUBLIC

Client::Client(): _status(NULLPTR),\
	_buffer_request(std::string()),\
	_fd(int()), \
	_server(), \
	_response(Response()), \
	_request(Request())
{}

Client::Client(int fd): _status(NULLPTR),\
	_buffer_request(std::string()),\
	_fd(fd), \
	_server(), \
	_response(Response()), \
	_request(Request())
{}

Client::~Client() {};

Client::Client(Client const &x) {
	*this = x;
}

Client	&Client::operator=(Client const &client) {
	this->_status = client._status;
	this->_data_socket = client._data_socket;
	this->_buffer_request = client._buffer_request;
	this->_fd = client._fd;
	this->_server = client._server;
	this->_response = client._response;
	this->_request = client._request;
	return (*this);
}

enum Status	Client::get_status() {
		return (_status);
}

std::string	Client::get_response() {
	ParseRequest	parser;

	parser.open(_buffer_request);
	_request = parser.get_request();
	_url.parse(_request.get_url_string());
	std::cout << _url.get_path() << "\n";
	_response.set_request(_request);
	shape_the_response();
	return (_response.get_response());
}

Request  &Client::get_request() {
	return (_request);
}

int			Client::get_fd() {
	return (_fd);
}

struct sockaddr_in	Client::get_data_socket() {
	return (_data_socket);
}

std::string			Client::get_buffer_request() {
	return (_buffer_request);
}

void		Client::set_data_socket(struct sockaddr_in data_socket) {
	_data_socket = data_socket;
}

void		Client::set_status(enum Status x) {
	this->_status = x;
}

void		Client::set_request(string headlers, int flag) {
	if (flag == 1) {
		this->_buffer_request = headlers;
	} else {
		this->_buffer_request += headlers;
	}
}

void		Client::set_fd(int const &x) {
	this->_fd = x;
}

void		Client::set_server(Server server) {
	_server = server;
}

void		Client::close_fd(void) {
	close(_fd);
}

// PRIVATE

void		Client::shape_the_response(void) {
	vector_string	shredded_url;
	Server	server;

//	 if (_url.get_path() == "favicon.ico") {
//	 	return ;
//	 }
	
	shredded_url = split_line(_url.get_path(), "/");
	server = find_location(_server, shredded_url); // shredded_url обрезается
	// std::cout << "$$$$$$$$$$$\n";
	// print_server(server);
	// std::cout << "$$$$$$$$$$$\n";
	if (check_error_max_body(server)) {
		error_run(server, 413);
	} else if (check_error_allow_methods(server.get_allow_methods())) {
		error_run(server, 405);
	} else if (_request.get_method() == "DELETE") {
		method_delete_run(server, shredded_url);
	} else if (_request.get_method() == "PUT") {
		method_put_run(server, shredded_url);
	} else if (_request.get_method() == "POST") {
		method_post_run(server, shredded_url);
	} else if (server.is_cgi_pass()) {
		cgi_run(server, shredded_url);
	} else if (server.is_redirect()) {
		redirect_run(server);
	} else if (server.is_autoindex()) {
        autoindex_run(server, shredded_url);
    }else if (server.is_authorization(_request)){
        if (authorization_run(_request.get_values_header("Authorization"), server)) {
            shape_the_response_for_logged_user(shredded_url, server);
        } else {
//            _response.set_code_status(403);
            _response.set_code_status(401);
//            index_run(server);
        }
	} else if (server.is_registration(_request.get_body())){
	    registration_run(_request.get_body());
    } else if (server.is_authorization(_request.get_body(), "Regist23&*&2rati43+_+-3H*74eon_01202*%^1(reg)(istr)ation")) {
	    authorization_run();
    } else if (server.is_index()) {
		index_run(server);
	} else {
		error_run(server, 404);
	}
}

void		Client::shape_the_response_for_logged_user(vector_string shredded_url, Server &server) {
    vector_string tmp = server.get_index();
    if (tmp.size()) {
        tmp[0] = "logged_" + tmp[0];
        server.set_index(tmp);
    }
    else {
        server.set_index(vector_string(1, "/logged_registration.html"));
        server.set_root("./other");
    }
    if (check_error_max_body(server)) {
        error_run(server, 413);
    } else if (check_error_allow_methods(server.get_allow_methods())) {
        error_run(server, 405);
    } else if (server.is_redirect()) {
        redirect_run(server);
    } else if (server.is_autoindex()) {
        autoindex_run(server, shredded_url);
    } else if (server.is_authorization(_request.get_body(), "log23&*&2rati43+_+-3H*74eon_01202*%^1(reg)(istr)ation")) {
        _response.set_code_status(401);
        index_run(server, server.get_root(), "/" + server.get_index()[0].erase(0, 8));
    } else if (server.is_index()) {
        index_run(server);
    } else {
        error_run(server, 404);
    }
}

void     Client::authorization_run() {
    _response.set_authorization(true);
    _response.set_code_status(401);
    _response.set_body_message("");

 }

bool    Client::authorization_run(std::vector<string> value_header, Server server) {
    unsigned char *decbuf =(unsigned char *)malloc(sizeof(unsigned char) * value_header[1].size());
    decode_b64((unsigned char*)value_header[1].c_str(), value_header[1].size(), decbuf);
    string user_data = "Login: ";
    int i = 0;

    for (; decbuf[i] != ':'; i++) {
        user_data += decbuf[i];
    }
    user_data += "\n";
    user_data += "Password: ";
    ++i;
    for (; decbuf[i] != 0; i++) {
        user_data += decbuf[i];
    }
    free(decbuf);

    int fd = open("other/user_data.txt", O_RDONLY);
    if (fd < 0) {
        std::cout << "Error: open fd client.cpp\n";
        exit(1);
    }
    string tmp = File::read_file(fd);
    bool res = check_data_user(tmp,user_data);
    if (!res) {
        return (false);
    }
    return (true);
}

bool    Client::check_data_user(string str_in, string str_find) {
    int i = 0;
    int a;

    while (1) {
       a = 0;
        if (str_in[i] == 'L') {
            while (str_in[i + a] != 0 && str_find[a] != 0 && str_in[i + a] == str_find[a]) {
                ++a;
            }
            if (str_find[a] == 0)
                return (true);
        }
        if (str_in[i + a] == 0)
            return (false);
        ++i;
    }
}

void	Client::cgi_run(Server &server, vector_string shredded_path) {
	CGI cgi;
	vector_string	shredded_root_path;
	string			path_cgi_script;

	if (shredded_path.size() == 0) {
		error_run(server, 404);
		return ;
	}
	if (server.is_root()) {
		shredded_root_path.push_back(server.get_root());
	}
	shredded_root_path.insert(shredded_root_path.end(), shredded_path.begin(), shredded_path.end());
	path_cgi_script = glue_link(shredded_root_path);
	cgi.set_url(_url);
	cgi.set_server(server);
	cgi.set_request(_request);
	cgi.set_expansion("plain");
	cgi.set_filename_script(path_cgi_script);
	cgi.set_data_socket_client(_data_socket);
	_response.set_code_status(200);
	_response.set_body_message(cgi.start());
}

void     Client::registration_run(string body)  {
    string data;
    int i = 18;
    for (; body[i] != 0; i++){
        data += body[i];
    }
    int fd = open("other/user_data.txt", O_RDONLY);
    if (fd < 0) {
        std::cout << "Error: open fd client.cpp\n";
        exit(1);
    }
    string tmp = File::read_file(fd);
    bool res = check_data_user(tmp, data);
    if (!res) {
        tmp = tmp + "\n\n" + data + " ";
        fd = open("other/user_data.txt", O_WRONLY);
        write(fd, tmp.c_str(), tmp.size());
    }
    _response.set_code_status(200);
}

void			Client::method_post_run(Server &server, vector_string shredded_path) {
	File			file;
	vector_string	shredded_root_path;

	if (server.is_root()) {
		shredded_root_path.push_back(server.get_root());
	}
	shredded_root_path.insert(shredded_root_path.end(), shredded_path.begin(), shredded_path.end());
	if (file.open_file(shredded_root_path) != 0) {
		error_run(server, 500);
		return ;
	}
	if (file.append_file(_request.get_body()) != 0) {
		error_run(server, 500);
		return ;
	}
	_response.set_code_status(200);
}

void			Client::method_put_run(Server &server, vector_string shredded_path) {
	File			file;
	vector_string	shredded_root_path;

	if (server.is_root()) {
		shredded_root_path.push_back(server.get_root());
	}
	shredded_root_path.insert(shredded_root_path.end(), shredded_path.begin(), shredded_path.end());
	if (file.open_file(shredded_root_path) != 0) {
		error_run(server, 500);
		return ;
	}
	if (file.write_file(_request.get_body()) != 0) {
		error_run(server, 500);
		return ;
	}
	_response.set_code_status(200);
}

void			Client::method_delete_run(Server &server, vector_string shredded_path) {
	File			file;
	vector_string	shredded_root_path;

	if (server.is_root()) {
		shredded_root_path.push_back(server.get_root());
	}
	shredded_root_path.insert(shredded_root_path.end(), shredded_path.begin(), shredded_path.end());
	if (file.open_file(shredded_root_path) != 0) {
		error_run(server, 404);
		return ;
	}
	if (file.delete_file() != 0) {
		error_run(server, 500);
		return ;
	}
	_response.set_code_status(200);
}

void		Client::index_run(Server &server) {
	File		file;
	string		text;
	int			code_error;
	
	code_error = file.open_file(server.get_root(), server.get_index()[0]);
	if (code_error > 0) {
		error_run(server, 404);
		return ;
	}
	text = file.read_file();
	_response.set_code_status(200);
	_response.set_body_message(text);
}

void		Client::index_run(Server &server, string root, string name_file) {
    File		file;
    string		text;
    int			code_error;

    code_error = file.open_file(root, name_file);
    if (code_error > 0) {
        error_run(server, 404);
        return ;
    }
    text = file.read_file();
    _response.set_code_status(200);
    _response.set_body_message(text);
}

void		Client::autoindex_run(Server &server, vector_string shredded_url) {
	vector_string	shredded_url_location;
	string			directory;
	string			url_location;
	Autoindex		autoindex;
	Response		response;
	File			file;
	string			text;

	shredded_url_location = split_line(_url.get_path(), "/");
	shredded_url_location = vector_string(shredded_url_location.begin(),
							shredded_url_location.end() - shredded_url.size());
	directory = glue_link(shredded_url);
	url_location = glue_link(shredded_url_location);

	if (is_directory("./" + directory) == false) {
		file.open_file(server.get_root(), directory);
		text = file.read_file();
		_response.set_expansion("plain");
	} else {
		autoindex.set_url_location(url_location);
		autoindex.set_root(server.get_root());
		autoindex.set_directory(directory);
		text = autoindex.get_html();
		_response.set_expansion("html");
	}
	_response.set_code_status(200);
	_response.set_body_message(text);
}

void		Client::redirect_run(Server &server) {
	_response.set_code_status(server.get_code_redirect());
	_response.set_url_redirect(server.get_url_redirect());
}

void	Client::error_run(Server &server, int code_error) {
	ErrorPage error_page;

	// if (server.is_error_page()) {;
	// 	error_page.set_error_pages(server.get_error_pages());
	// }
	if (server.is_root()) {
		error_page.set_root_error_page(server.get_root());
	}
	error_page.set_code_error(code_error);

	_response.set_error_page(error_page);
}

Server& Client::find_location(Server &server, vector_string &shredded_url) {
	vector_string keys_locations;
	int index = 0;
	int count_coincidence = -1;
	int tmp;
	vector_string shredded_url_location;

	std::cout << "\n^^^^^^^^^^^^^^^^^^^\n";


	keys_locations = server.get_keys_locations();
	for (int i = 0; i < keys_locations.size(); i++) {
		shredded_url_location = split_line(keys_locations[i], "/");
		tmp = find_count_coincidence(shredded_url_location, shredded_url);
		if (tmp > count_coincidence) {
			count_coincidence = tmp;
			index = i;
		} else if (tmp != -1 && tmp == count_coincidence) {
			vector_string one = vector_string(shredded_url.begin() + count_coincidence, shredded_url.end());
			vector_string two = one;
			Server server_one = server.get_location(index);
			Server server_two = server.get_location(i);
			find_location(server_one, one);
			find_location(server_two, two);
			if (one.size() > two.size()) {
				index = i;
			}
		}
	}
	if (count_coincidence != -1) {
		Server &location = server.get_location(index);
		shredded_url = vector_string(shredded_url.begin() + 
			count_coincidence, shredded_url.end());
		return (find_location(location, shredded_url));
	}
	return (server);
}

int		Client::find_count_coincidence(vector_string shredded_url_location, vector_string shredded_url) {
	int i = 0;

	for (; i < shredded_url_location.size() && i < shredded_url.size(); i++) {
		if (shredded_url_location[i] != shredded_url[i]) {
			break ;
		}
	}
	if (i < shredded_url_location.size()) {
		return (-1);
	}
	return (i);
}

bool	Client::check_error_max_body(Server &server) {
	if (_request.get_body().size() > server.get_max_body_size()) {
		return (true);
	}
	return (false);
}

bool	Client::check_error_allow_methods(vector_string allow_methods) {
	print_vector("allow_methods:", allow_methods);
	if (find_word(allow_methods, _request.get_method()) == -1) {
		return (true);
	}
	return (false);
}

string	Client::glue_link(vector_string &shredded_url) {
	string directory;

	for (int i = 0; i < shredded_url.size(); i++) {
		directory += shredded_url[i];
		directory += (i + 1 == shredded_url.size()) ? "" : "/";
	}
	return (directory);
}

bool Client::is_directory(string path) {
	string location;

	location = ft_getcwd();
	if (chdir(path.c_str()) == 0) {
		chdir(location.c_str());
		return (true);
	}
	return (false);
}

string Client::ft_getcwd(void) {
	int size = 1;

    while (1) {
        char dir[size];

        if (getcwd(dir, size)) {
            return (string(dir));
            break ;
        }
        size++;
    }
	return ("");
}
