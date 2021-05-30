#include "header.hpp"
// #include "./parse_confige/Server.hpp"

int main(int argc, char **argv) {
    std::string         confige;
    std::vector<Server> servers;
    ParseConfige        parser;
    HandlerConnects     handler_connects;

    if (argc >= 2) {
        confige = string(argv[1]);
    } else {
        confige = "./other/configurations/webserv.conf";
    }
    try {
        parser.open(confige);
        servers = parser.get_servers();
        handler_connects.start(servers);
    } catch(std::exception &error) {
        std::cout << "Error: " << error.what() << std::endl;
    }
    return (0);
}