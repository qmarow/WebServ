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
			this->_clients.push_back(client);
		}
	}
}

string	read_up_to_string(int fd, string s){
	string tmp;
	char buff[2];
	int count_read;

	bzero(&buff, 2);
	while (1) {
		count_read = recv(fd, buff, 1, 0);
		if (count_read == 0) {
			break ;
		}
		if (count_read == -1) {
			std::cout << "\n\tERROR in s\n";
			usleep(5000000);
		}
		std::cout << "buff - |" << buff[0] << "| - " << (int)buff[0] << "\n";
		tmp += buff[0];
		if (tmp.find(s) != -1 || buff[0] == 0) {
			break ;
		}
	}
	return (tmp);
}

int 	read_heandler(Client &client){
	string tmp;
	int i = 0;
	char buff[2];
	int fd = client.get_fd();
	int count_read;

	bzero(&buff, 2);
	std::cout << "read_heandler\n";
	while (1) {
		count_read = recv(client.get_fd(), buff, 1, 0);
		std::cout << count_read << "\n";
		if (count_read == 0) {
		    return (0);
		}
		if (count_read == -1) {
			std::cout << "\n\tERROR\n";
			exit(1);
		}
		tmp += buff[0];
		// if (i > 0 && tmp[i] == '\n' && tmp[i - 1] == '\n') {
		// 	break ;
		// }
		if (tmp.find("\r\n\r\n") != -1) {
			break ;
		}
		++i;
	}
	std::cout << "ЗАПРОС ЗАГОЛОВКИ \n|" << tmp << "|\n";
	client.set_request(tmp, "");
	return (count_read);
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

int	read_body_message(Client &client) {
	string	buffer_request = client.get_buffer_request();
	int i = find_in_headers(buffer_request, "Content-Length");
    int count_read;


    if (i != -1) {
		char buff[i];
		i = get_number(buffer_request, i);
		bzero(&buff, i);
		count_read = recv(client.get_fd(), buff, i, 0);
		client.get_request().set_body(string(buff));
		client.set_status(WRITE);
	} else if ((i = find_in_headers(buffer_request, "Transfer-Encoding: chunked")) != -1){
		string tmp = read_up_to_string(client.get_fd(), "\r\n\r\n");
		std::cout << "/|" << tmp << "|\\\n";
        i = std::stoi(tmp);
        if (i != 0) {
            char buff[i];
            bzero(&buff, i);
            count_read = recv(client.get_fd(), buff, i, 0);
            client.get_request().set_body(string(buff));
			// client.set_status(NOTWRITEHEADLER);
        } else {
			std::cout << "Transfer write\n";
            client.set_status(WRITE);
        }
	} else {
		client.get_request().set_body("");
		client.set_status(WRITE);
	}
    return (count_read);
}

void	HandlerConnects::parse_client(iter_client &it) {
	if (read_heandler(*it)) {
        read_body_message(*it);
    } else {
		std::cout << "!!!!!!!!!!!!!'\n";
		it->close_fd();
		_clients.erase(it);
	}
}

int		HandlerConnects::response_for_client(Client &client) {
	string	response;

	response = client.get_response();
	int i = response.find("\n\n");
	i += 3;
	if (response[i] != 0 || client.get_request().get_method() == "GET") {
		i = 1;
	} else {
		i = 0;
	}
	std::cout << "\n\n|" << response << "|\n";
	int err = send(client.get_fd(), response.c_str(), response.size(), 0);
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
				it->close_fd();
				_clients.erase(it);
				continue;
			} else {
				it->clear();
			}
			it->set_status(NULLPTR);
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
