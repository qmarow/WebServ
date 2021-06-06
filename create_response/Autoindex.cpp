#include "Autoindex.hpp"
#include "./../utils/UtilsPrint.ipp"

Autoindex::Autoindex() {
    _root = string();
    _directory = string();
}

Autoindex::Autoindex(const Autoindex &autoindex) {
    *this = autoindex;
}

Autoindex &Autoindex::operator=(const Autoindex &autoindex) {
    _root = autoindex._root;
    _directory = autoindex._directory;
    return (*this);
}

Autoindex::~Autoindex() {}

void Autoindex::set_root(string root) {
    _root = trim_line(root, "/");
}

void Autoindex::set_directory(string directory) {
    _directory = trim_line(directory, "/");
}

void Autoindex::set_url_location(string url) {
    _url = trim_line(url, "/");
}

string Autoindex::get_html() {
    string      html;
    vector_pair directories;

    directories = get_directories();
    html += "<!DOCTYPE html>\n";
    html += "<html>\n";
    html +=     "\t<head>\n";
    html +=         "\t\t<title>\n";
    html +=             "\t\t\t" + get_html_title() + "\n";
    html +=         "\t\t</title>\n";
    html +=     "\t</head>\n";
    html +=     "\t<body>\n";
    html +=         "\t\t<h1>Index: </h1>\n";
    html +=         "\t\t<ul>\n";
    html +=             "\t\t\t" + get_html_directories(directories) + "\n";
    html +=         "\t\t</ul>\n";
    html +=     "\t</body>\n";
    html += "</html>\n";
    return (html);
}


const char * Autoindex::NotFoundException::what() const throw() {
    return ("Not found!");
}

// PRIVATE

Autoindex::vector_pair Autoindex::get_directories(void) {
    vector_pair     pairs;
    vector_string   directory_names;
    string          href;

    directory_names = get_directory_names();
    for (int i = 0; i < directory_names.size(); i++) {
        href = get_href(directory_names[i]);
        pairs.push_back(pair(directory_names[i], href));
    }
    return (pairs);
}

Autoindex::string   Autoindex::get_href(string object) {
    string href;
    string url_location = _url;
    string path_directory = _directory;

    if (url_location.size()) {
        url_location = "/" + url_location;
    }
    if (path_directory.size()) {
        path_directory = "/" + path_directory;
    }
    if (object == ".") {
        href = url_location + path_directory;
    } else if (object == "..") {
        href = get_href_back(url_location, path_directory);
    } else {
        href = url_location + path_directory + "/" + object;
    }
    return (href);
}

Autoindex::string           Autoindex::get_href_back(string url_location, string path_directory) {
    string href;
    vector_string directories;
    vector_string shredded_url;

    href = url_location;
    directories = split_line(path_directory, "/");
    if (directories.size() != 0) {
        for (int i = 0; i < (int)(directories.size() - 1); i++) {
            href += "/" + directories[i];
        }
        return (href);
    }
    href = "";
    shredded_url = split_line(url_location, "/");
    for (int i = 0; i < (int)(shredded_url.size() - 1); i++) {
        href += "/" + shredded_url[i];
    }
    if (href.size() == 0) {
        href = "/";
    }
    return (href);
}

Autoindex::vector_string    Autoindex::get_directory_names(void) {
    struct dirent   *struct_dirent;
    DIR             *directory;
    string          path;
    vector_string   directory_names;

    path = _root + "/" + _directory;
    directory = opendir((path).c_str());
    if (directory == NULL) {
        throw NotFoundException();
    }
    while ((struct_dirent = readdir(directory)) != NULL) {
        directory_names.push_back(string(struct_dirent->d_name));
    }
    closedir (directory);
    return (directory_names);
}

Autoindex::string   Autoindex::get_html_directories(vector_pair directories) {
    string html;

    for (int i = 0; i < directories.size(); i++) {
        html += get_html_directory(directories[i]);
    }
    return (html);
}

Autoindex::string  Autoindex::get_html_title(void) {
    return ("");
}

Autoindex::string  Autoindex::get_html_directory(pair directory) {
    string html;

    html = "<li>\n";
    html +=     "\t<a href=\"" + directory.second + "\">\n";
    html +=         "\t\t" + directory.first + "\n";
    html +=     "\t</a>\n";
    html += "</li>\n";
    return (html);
}