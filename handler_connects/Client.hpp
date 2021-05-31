#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client;

#include <string>
#include <fcntl.h>
#include "./../parse_confige/Server.hpp"
#include "../utils/File.hpp"
#include "./Autoindex.hpp"
#include "./../parse_url/URL.hpp"
#include "./../utils/Structs.hpp"

enum	Status {
	READ,
	WRITE,
	CLOSE,
	NULLPTR
};

class Client {
public:
	typedef std::string string;

	Client();
	~Client();
	Client(int fd);
	Client(Client const &x);
	Client	&operator=(Client const &x);

private:
	enum Status				_status;
	struct sockaddr_in  	_data_socket;
	string					_buffer_request;
	int						_fd;
	Server					_server;

public:
	enum Status			get_status();
	string				get_response();
	int					get_fd();
	struct sockaddr_in	get_data_socket();
	void				set_status(enum Status x);
	void				set_request(char *s);
	void				set_fd(int const &x);
	void				set_server(Server server);
	void				set_data_socket(struct sockaddr_in data_socket);
	void				close_fd(void);

private:
	string		shape_the_response(Request &request, URL &url);
	string      registration_run(string body);
	string      authorization_run();
	string		index_run(Server &server);
	string		autoindex_run(Server &server, URL &url, vector_string shredded_url);
	string		redirect_run(Server &server);
	bool		check_error_max_body(Server &server, Request &request);
	bool		check_error_allow_methods(vector_string allow_methods, Request &request);
	string		error_run(Server &server, int code_error);
	Server		&find_location(Server &server, vector_string &shredded_url);
	int			find_count_coincidence(vector_string shredded_url_location,
										vector_string shredded_url);
	string		glue_link(vector_string &shredded_url);

	// перенесьти
	bool	is_directory(string path);
	string	ft_getcwd(void);

	// delete
	string response_http(string body);
};

#endif
