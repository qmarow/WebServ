template <class T>
void print_vector(const std::string &title, const T &vector) {
    std::cout << title << " ";
    for (int i = 0; i < vector.size(); i++) {
        std::cout << vector[i] << "|";
    }
    std::cout << std::endl;
}

template <class T>
void print_locations(std::string name_location, T location, int count_tab) {
    std::string tabs(count_tab, '\t');

    std::cout << tabs << "ip: " << location.get_ip() << std::endl;
    std::cout << tabs <<  "port: " << location.get_port() << std::endl;
    if (location.is_max_body_size()) {
        std::cout << tabs << "max_body_size: " << location.get_max_body_size() << std::endl;
    }
    if (location.is_root()) {
        std::cout << tabs << "root: " << location.get_root() << std::endl;
    }
    if (location.is_server_name()) {
        std::cout << tabs << "server_name: " << location.get_server_name() << std::endl;
    }
    if (location.is_index()) {
        std::cout << tabs;
        print_vector("index:", location.get_index());
    }
    if (location.is_autoindex()) {
        std::cout << tabs << "autoindex on" << std::endl;
    } else {
        std::cout << tabs << "autoindex off" << std::endl;
    }
    for (int i = 0; i < location.get_cods_error_page().size(); i++) {
        int code = location.get_cods_error_page()[i];
        std::cout << tabs << "error_page: " << code << " | " << location.get_file_error_page(code) << std::endl;
    }
    if (location.is_redirect()) {
        std::cout << tabs << "return: " << location.get_code_redirect() << " | " << location.get_url_redirect() << std::endl;
    }
    if (location.is_timeout()) {
        std::cout << tabs << "timeout: " << location.get_timeout() << std::endl;
    }
    if (location.is_allow_methods()) {
        std::cout << tabs << "allow methods: ";
        for (int i = 0; i < location.get_allow_methods().size(); i++) {
            std::cout << location.get_allow_methods()[i] << " ";
        }
        std::cout << std::endl;
    }
    if (location.is_cgi_allow_methods()) {
        std::cout << tabs << "cgi allow methods: ";
        for (int i = 0; i < location.get_cgi_allow_methods().size(); i++) {
            std::cout << location.get_cgi_allow_methods()[i] << " ";
        }
        std::cout << std::endl;
    }
    // if (name_location.size() > 0) {
    //     std::cout << tabs << "location: " << name_location << std::endl;
    // }
    std::cout << std::endl;
    std::vector<std::string> keys_locations = location.get_keys_locations();
    for (int i = 0; i < keys_locations.size(); i++) {
        std::cout << tabs << "location " << keys_locations[i] << std::endl;
        print_locations(keys_locations[i], location.get_location(keys_locations[i]), count_tab + 1);
    }
}

template <class T>
void print_servers(std::vector<T> servers) {
    for (int k = 0; k < servers.size(); k++) {
        // print_vector("name location: ", servers[k].get_keys_locations());
        std::cout << "_____________" << k << "_____________\n";
        print_locations("", servers[k], 0);
    }
}

template <class T>
void print_server(T server) {
    print_locations("", server, 0);
}

template <class T>
void print_request(T request) {
    std::cout << request.get_method() << "|" << request.get_url_string() << "\n\n";
    std::vector<std::string> keys_headers = request.get_keys_headers();

    for (int i = 0; i < keys_headers.size(); i++) {
        std::cout << keys_headers[i] << ": ";
        std::vector<std::string> values = request.get_values_header(keys_headers[i]);
        int k = 0;
        for (; k < values.size(); k++) {
            std::cout << values[k];
            std::cout << ((k < values.size() - 1) ? ", " : "");
        }
        std::cout << std::endl;
    }

    std::cout << request.get_body() << std::endl;
}

template <class T>
void print_url(T url) {
    std::cout << "path: " << url.get_path() << std::endl;
    std::cout << "value: ";
    for (int i = 0; i < url.count_argv(); i++) {
        std::cout << url.at_keys(i) << ":" << url.at_values(i) << " ";
    }
    std::cout << std::endl;
}