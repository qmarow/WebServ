#include "Client.hpp"

// PUBLIC

Client::Client(): _status(NULLPTR),\
	_buffer_request(std::string()),\
	_fd(int()), \
	_server()
{};

Client::Client(int fd): _status(NULLPTR),\
	_buffer_request(std::string()),\
	_fd(fd), \
	_server()
{};

Client::~Client() {};

Client::Client(Client const &x) {
	*this = x;
}

Client	&Client::operator=(Client const &client)
{
	this->_buffer_request = client._buffer_request;
	this->_status = client._status;
	this->_fd = client._fd;
	this->_server = client._server;
	return (*this);
}

enum Status	Client::get_status() {
		return (_status);
}

std::string	Client::get_response() {
	ParseRequest	parser;
	Request			request;
	URL				url;

	parser.open(_buffer_request);
	request = parser.get_request();
	url.parse(request.get_url_string());
	return (shape_the_response(request, url));
}

int			Client::get_fd() {
	return (_fd);
}

struct sockaddr_in	Client::get_data_socket() {
	return (_data_socket);
}

void		Client::set_data_socket(struct sockaddr_in data_socket) {
	_data_socket = data_socket;
}

void		Client::set_status(enum Status x) {
	this->_status = x;
}

void		Client::set_request(char *s) {
	this->_buffer_request += std::string(s);
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

std::string		Client::shape_the_response(Request &request, URL &url) {
	if (url.get_path() == "favicon.ico") {
		return ("");
	}
	vector_string shredded_url = split_line(url.get_path(), "/");
	Server &server = find_location(_server, shredded_url);
	if (check_error_max_body(server, request)) {
		return (error_run(server, 413));
	}
	if (check_error_allow_methods(server.get_allow_methods(), request)) {
		return (error_run(server, 405));
	}
	if (server.is_redirect()) {
		return (redirect_run(server));
	} else if (server.is_autoindex()) {
		return (autoindex_run(server, url, shredded_url));
	} else if (server.is_index()) {
		return (index_run(server));
	} else if (server.is_authorization()) {
	    return ()
	}else {
		return (error_run(server, 404));
	}
}

void		cgi_run() {

}

std::string     Client::authorization_run() {

}

Server& Client::find_location(Server &server, vector_string &shredded_url) {
	vector_string keys_locations;
	int index = 0;
	int count_coincidence = 0;
	int tmp;
	vector_string shredded_url_location;

	keys_locations = server.get_keys_locations();
	for (int i = 0; i < keys_locations.size(); i++) {
		shredded_url_location = split_line(keys_locations[i], "/");
		tmp = find_count_coincidence(shredded_url_location, shredded_url);
		if (tmp > count_coincidence) {
			count_coincidence = tmp;
			index = i;
		} else if (tmp != 0 && tmp == count_coincidence) {
			vector_string one = vector_string(shredded_url.begin() + 
			count_coincidence, shredded_url.end());
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
	if (count_coincidence != 0) {
		Server &location = server.get_location(index);
		shredded_url = vector_string(shredded_url.begin() + 
			count_coincidence, shredded_url.end());
		return (find_location(location, shredded_url));
	}
	return (server);
}

int		Client::find_count_coincidence(vector_string shredded_url_location, vector_string shredded_url) {
	int i;

	for (i = 0; i < shredded_url_location.size()
				&& i < shredded_url.size(); i++) {
		if (shredded_url_location[i] != shredded_url[i]) {
			break ;
		}
	}
	if (i < shredded_url_location.size()) {
		return (0);
	}
	return (i);
}

std::string		Client::index_run(Server &server) {
	Response	response;
	File		file;
	string		text;
	int			code_error;
	
	code_error = file.openFile(server.get_root(), server.get_index()[0]);
	if (code_error == 1) {
		return (error_run(server, 404));
	} else if (code_error == 2) {
		return (error_run(server, 404));
	}
	text = file.readFile();
	response.set_code_status(200);
	response.set_body_message(text);
	file.closeFile();
	std::cout << response.get_response() << "\n";
	return (response.get_response());
}

std::string		Client::autoindex_run(Server &server, URL &url, vector_string shredded_url) {
	vector_string	shredded_url_location;
	string			directory;
	string			url_location;
	Autoindex		autoindex;
	Response		response;
	File			file;
	string			text;
	
	shredded_url_location = split_line(url.get_path(), "/");
	shredded_url_location = vector_string(shredded_url_location.begin(),
							shredded_url_location.end() - shredded_url.size());
	directory = glue_link(shredded_url);
	url_location = glue_link(shredded_url_location);

	if (is_directory("./" + directory) == false) {
		file.openFile(server.get_root(), directory);
		text = file.readFile();
		file.closeFile();
	} else {
		autoindex.set_url_location(url_location);
		autoindex.set_root(server.get_root());
		autoindex.set_directory(directory);
		text = autoindex.get_html();
	}
	response.set_code_status(200);
	response.set_body_message(text);
	return (response.get_response());
}

std::string		Client::redirect_run(Server &server) {
	Response response;

	response.set_code_status(server.get_code_redirect());
	response.set_url_redirect(server.get_url_redirect());
	return (response.get_response());
}

void		default_method_body_run() {

}

bool	Client::check_error_max_body(Server &server, Request &request) {
	if (request.get_body().size() > server.get_max_body_size()) {
		return (true);
	}
	return (false);
}

bool	Client::check_error_allow_methods(vector_string allow_methods, Request &request) {
	if (find_word(allow_methods, request.get_method()) == -1) {
		return (true);
	}
	return (false);
}

string	Client::error_run(Server &server, int code_error) {
	Response response;
	ErrorPage error_page;

	error_page.set_error_pages(server.get_error_pages());
	error_page.set_root_error_page(server.get_root());
	error_page.set_code_error(code_error);

	response.set_code_status(code_error);
	response.set_body_message(error_page.get_error_page());
	return (response.get_response());
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
