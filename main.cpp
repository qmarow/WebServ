#include <string>
#include <vector>
#include "./parse_confige/Server.hpp"
#include "./parse_confige/ParseConfige.hpp"
#include "./handler_connects/HandlerConnects.hpp"
#include "./utils/File.hpp"

int main(int argc, char **argv, char **env) {
    std::string         confige;
    std::vector<Server> servers;
    ParseConfige        parser;
    HandlerConnects     handler_connects;

    if (argc >= 2) {
        confige = std::string(argv[1]);
    } else {
        confige = "./other/configurations/webserv.conf";
    }
    try {
        parser.open(confige);
        servers = parser.get_servers();
        // print_servers(servers);
        handler_connects.start(servers);
    } catch(std::exception &error) {
        std::cout << "Error: " << error.what() << std::endl;
    }
    return (0);
}