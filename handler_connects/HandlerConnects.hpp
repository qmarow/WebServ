#ifndef HANDLERCONNECTS_HPP
#define HANDLERCONNECTS_HPP

#include "./Client.hpp"
#include "./../utils/TransferNumberSystem.hpp"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

class HandlerConnects {
public:
	typedef std::string				string;
    typedef std::vector<int>		vector_fd;
	typedef std::vector<Server>		vector_server;
	typedef std::vector<Client>		vector_client;
	typedef vector_client::iterator	iter_client;

private:
	vector_fd		_fds_master; // delete
	vector_server	_servers;
	vector_client	_clients;
	int				_max_fd;
	fd_set			_read_set;
	fd_set			_write_set;

public:
	HandlerConnects();
	HandlerConnects(const HandlerConnects &handler_connects);
	HandlerConnects &operator=(const HandlerConnects &handler_connects);
	~HandlerConnects();

	void	start(vector_server &servers);

private:
	int     get_socket(string host, int port);
	int     get_max_fd(vector_fd fds);

	void	update_clients();
	void	init_set_client(iter_client client);
	void	data_preparation(vector_server &servers);
	void	handler_connect_client();
	void	handler_set_reads();
	void	handler_set_writes();
	int		response_for_client(Client &client);						
	void	parse_client(iter_client &it);
};

#endif