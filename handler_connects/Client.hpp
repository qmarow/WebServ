#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client;

#include <string>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "./../parse_confige/Server.hpp"
#include "./../utils/File.hpp"
#include "./../create_response/Autoindex.hpp"
#include "./../parse_url/URL.hpp"
#include "./../utils/Structs.hpp"
#include "./../parse_request/Request.hpp"
#include "./../parse_request/ParseRequest.hpp"
#include "./../create_response/Response.hpp"
#include "./../create_response/ErrorPage.hpp"

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
	Response				_response;
	Request					_request;
	URL						_url;

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
    bool        check_data_user(string str_in, string str_find);
	void		shape_the_response(void);
	void        registration_run(string body);
	void        authorization_run();
    void        authorization_run(std::vector<string>);
    void		index_run(Server &server);
	void		autoindex_run(Server &server, vector_string shredded_url);
	void		redirect_run(Server &server);
	bool		check_error_max_body(Server &server);
	bool		check_error_allow_methods(vector_string allow_methods);
	void		error_run(Server &server, int code_error);
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
