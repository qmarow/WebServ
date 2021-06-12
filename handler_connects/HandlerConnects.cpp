#include "HandlerConnects.hpp"

HandlerConnects::HandlerConnects() {
	_fds_master = vector_fd();
	_clients = vector_client();
	_max_fd = 0;
};

HandlerConnects::~HandlerConnects() {};

int                 HandlerConnects::get_max_fd(vector_fd fds) {
	int max = 0;

	for (int i = 0; i < fds.size(); i++) {
		if (max < fds[i]) {
			max = fds[i];
		}
	}
	return (max);
}

int		HandlerConnects::get_socket(string host, int port) {
	std::cout << "___get_socket()___\n";
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

void	HandlerConnects::data_preparation(vector_server &servers) {
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
			std::cout << "ADD new client\n";
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
			client._flag = false;
			this->_clients.push_back(client);
		}
	}
}

int		checking_readed_for_transfer_encoding(string buffer_request){
	int i = buffer_request.find("\r\n\r\n");
	int result = 0;

	i += 4;
	for (; buffer_request[i] != '\r'; i++) {
		if (buffer_request[i] == 0) {
			return (-1);
		}
		result *= 10;
		result += (buffer_request[i] - '0');
	}
	// if (result == 0) {
	// 	return (result);
	// }
	std::cout << "^/^\n";
	result += 4;
	while (buffer_request[i] != 0 && result--) {
		++i;
	}
	return (result);
}

int		checking_readed_for_content_length(string buffer_request, int content_length){
	int i = buffer_request.find("\r\n\r\n");
	int result = 0;

	i += 5;
	for (; buffer_request[i] != 0; i++) {
		--content_length;
	}
	if (content_length <= 0) {
		return (0);
	}
	return (1);
}

int 	read_heandler(Client &client){
	string tmp;
	char buff[101];
	int fd = client.get_fd();
	int count_read;

	bzero(&buff, 101);
	std::cout << "read_heandler\n";
	count_read = recv(client.get_fd(), buff, 100, 0);
	if (count_read == 0) {
	    return (0);
	}
	if (count_read == -1) {
		std::cout << "\n\tERROR\n";
		exit(1);
	}
	tmp.append(buff);
	client.set_request(tmp, 0);
	if (client.get_buffer_request() != "" && client.get_buffer_request().find("\r\n\r\n") != -1) {
		return (1);
	}
	 //    если 0 то += если 1 то =

	return (2);
}

int		get_number(string buff, int i) {
	int res = 0;
	while (buff[i] != 0 && !isdigit(buff[i])) {
		++i;
	}
	res = buff[i] - '0';
	while (isdigit(buff[i])){
		res *= 10;
		res += buff[i] - '0';
		++i;
	}
	return (res);
}

void	read_body_message(Client &client) {
	string	buffer_request = client.get_buffer_request();
	int i = find_in_headers(buffer_request, "Content-Length: ");
    int count_read;


			std::cout << "read_body_message\n";

    if (i != -1) {
		i += 16;
		i = get_number(buffer_request, i);
		if (checking_readed_for_content_length(buffer_request, i)) {
			return ;
		}
		client.set_status(WRITE);
	} else if ((i = find_in_headers(buffer_request, "Transfer-Encoding: chunked")) != -1){
		int tmp = checking_readed_for_transfer_encoding(buffer_request);
		std::cout << "RES TR-EN - " << tmp << "\n";
		if (tmp != 0) {
			return ;
		}
		std::cout << "Transfer write\n";
        client.set_status(WRITE);
	} else {
		std::cout << "OTHER\n";
		// client.get_request().set_body("");
		client.set_status(WRITE);
	}
}

void	HandlerConnects::parse_client(iter_client &it) {
	int result = read_heandler(*it);
	if (result == 1) {
		std::cout << "after read_headnler one if\n";
        read_body_message(*it);
    } else if (result == 0){
		it->close_fd();
		_clients.erase(it);
	}

	std::cout << "ЗАПРОС ЗАГОЛОВКИ \n|" << it->get_buffer_request() << "|\n";

}

int		HandlerConnects::response_for_client(Client &client) {
	string	response;

	response = client.get_response();
	std::cout << "\n\n|" << response << "|\n";
	int i = response.find("\r\n\r\n");
	i += 5;
	if (response[i] != 0) {
		i = 1;
	} else {
		i = 0;
	}
	int err = send(client.get_fd(), response.c_str(), response.size(), 0);
	client.set_request("", 1);
	return (i);
}

void	HandlerConnects::handler_set_reads() {
	iter_client it = _clients.begin();
	for (; it < _clients.end(); ++it) {
		if (FD_ISSET(it->get_fd(), &_read_set)) {
			std::cout << "PARSE client\n";
			parse_client(it);
		}
	}
}

void		HandlerConnects::handler_set_writes() {
	iter_client it = _clients.begin();

	while (it != _clients.end()) {
		if (FD_ISSET(it->get_fd(), &_write_set)) {
			std::cout << "WRITE client\n";
			if (response_for_client(*it)) {
				std::cout << "DELETE CONNECTIOIN for CLIENT\n";
				it->close_fd();
				_clients.erase(it);
				continue;
			}
			it->set_status(NULLPTR);
		}
		it++;
	}
}

void	HandlerConnects::handler_connect_client() {
	std::cout << "select wait...\n";
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
	std::cout << "init_client\n";
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
