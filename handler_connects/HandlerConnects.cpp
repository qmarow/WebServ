#include "HandlerConnects.hpp"

HandlerConnects::HandlerConnects()
{
	_fds_master = vector_fd();
	_clients = vector_client();
	_max_fd = 0;
};

HandlerConnects::~HandlerConnects()
{};

int                 HandlerConnects::get_max_fd(vector_fd fds) {
	int max = 0;

	for (int i = 0; i < fds.size(); i++) {
		if (max < fds[i]) {
			max = fds[i];
		}
	}
	return (max);
}

int		HandlerConnects::get_socket(string host, int port)
{
	struct sockaddr_in	addr;
	int 				optval = 1;
	int					master_sock;

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host.c_str());

	if ((master_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error socket()\n");
		return (-1);
	}
	fcntl(master_sock, F_SETFL, O_NONBLOCK);

	// Допускает повторное использование локального адреса
	setsockopt(master_sock, SOL_SOCKET,  SO_REUSEADDR, &optval, sizeof(optval));
	// // контролирует генерацию SIGPIPE для сокета. В POSIX-системах, SIGPIPE — сигнал,
	// // посылаемый процессу при записи в соединение (пайп, сокет) при отсутствии или 
	// // обрыве соединения с другой (читающей) стороной.
	// setsockopt(master_sock, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof (optval));
	// // Периодически проверяет, находится ли соединение в рабочем состоянии
	// setsockopt(master_sock, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof (optval));

	if ((bind(master_sock, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
		printf("Error bind()\n");
		return (-1);
	}
	if ((listen(master_sock, 1000)) < 0) {
		printf("Error listen()\n");
		return (-1);
	}
	return (master_sock);
}

void	HandlerConnects::data_preparation(vector_server &servers)
{
	std::string	ip;
	int			port;
	int			socket_master;

	_servers = servers;
	for (int i = 0; i < servers.size(); i++)
	{
		ip = servers[i].get_ip();
		port = servers[i].get_port();

		std::cout << ip << ":" << port << std::endl;
		socket_master = get_socket(ip, port);
		if (socket_master != -1) {
			this->_fds_master.push_back(socket_master);
		}
	}
}

void	HandlerConnects::update_clients() {
	struct sockaddr_in addr;
	socklen_t	len;
	int			fd_client;

	for (int i = 0; i < _fds_master.size(); ++i) {
		if (FD_ISSET(_fds_master[i], &_read_set)) {
			fd_client = accept(_fds_master[i], (struct sockaddr*)&addr, &len); 
			if (fd_client < 0) {
				std::cout << "Error: accept client\n";
				continue;
			}
			if (fd_client > _max_fd) {
				_max_fd = fd_client;
			}
			fcntl(fd_client, F_SETFL, O_NONBLOCK);
			Client client(fd_client);
			client.set_server(_servers[i]);
			client.set_data_socket(addr);
			this->_clients.push_back(client);
		}
	}
}

void	HandlerConnects::parse_client(Client &client) {
	char buff[100];
	int count_read;

	bzero(&buff, 100);

	std::cout << "RESUEST:\n[\033[32;4;24m";
	while ((count_read = recv(client.get_fd(), buff, sizeof(buff), 0)) > 0) {
		buff[count_read] = '\0';
		client.set_request(buff);
		std::cout << buff << "\n";
	}
	std::cout << "\033[0m]\n";
	client.set_status(WRITE);
}

void	HandlerConnects::response_for_client(Client &client) {
	string	response;
	
	response = client.get_response();
	std::cout << "RESPONSE:\n[\033[31;1;4m" << response << "\n\033[0m";
	int err = send(client.get_fd(), response.c_str(), response.size(), 0);
}

void	HandlerConnects::handler_set_reads() {
	for (int i = 0; i < _clients.size(); i++) {
		if (FD_ISSET(_clients[i].get_fd(), &_read_set)) {
			parse_client(_clients[i]);
		}
	}
}

void		HandlerConnects::handler_set_writes() {
	iter_client it = _clients.begin();
	while (it != _clients.end()) {
		if (FD_ISSET(it->get_fd(), &_write_set)) {
			response_for_client(*it);
			it->close_fd();
			it = _clients.erase(it);
			continue;
		}
		it++;
	}
}

void	HandlerConnects::handler_connect_client() {
	if (select(_max_fd + 1, &_read_set, &_write_set, NULL, NULL) < 0) {
		std::cout << "Error: select";
		return ;
	}
	update_clients();
	handler_set_reads();
	handler_set_writes();
}

void	HandlerConnects::init_set_client(iter_client client) {
	FD_SET(client->get_fd(), &_read_set);

	if (client->get_status() == WRITE) {
		FD_SET(client->get_fd(), &_write_set);
	}
}

void	HandlerConnects::start(vector_server &servers) {
	iter_client	it_client;

	data_preparation(servers);
	_max_fd = get_max_fd(_fds_master);
	while (1) {
		it_client = _clients.begin();
		FD_ZERO(&_read_set);
		FD_ZERO(&_write_set);
		for (int i = 0; i < _fds_master.size(); ++i)
			FD_SET(_fds_master[i], &_read_set);
		while (1) {
			if (_clients.empty() || it_client == _clients.end()) {
				handler_connect_client();
				break ;
			} else {
				init_set_client(it_client);
				it_client++;
			}
		}
	}
}
