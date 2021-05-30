#include "File.hpp"

// PUBLIC

File::File() {
    _fd = -1;
}

File::File(const File &file) {
    *this = file;
}

File &File::operator=(const File &file) {
    _fd = file._fd;
    return (*this);
}

File::~File() {
    if (_fd != -1) {
        close(_fd);
    }
    
}

int		File::openFile(string root, string name) {
    string file;
    
    file = get_file_with_path(root, name);
    _fd = open(file.c_str(), O_RDONLY);
    if (_fd == -1) {
        return (2);
    }
    return (0);
}

std::string	File::readFile(void) {
    char	buffer[2];
    string	text;

    if (_fd == -1) {
        return ("");
    }
    while (read(_fd, buffer, 1) > 0) {
        buffer[1] = '\0';
        text += string(buffer);
    }
    return (text);
}

std::string	File::readFile(int fd) {
    char	buffer[2];
    string	text;

    if (fd == -1) {
        return ("");
    }
    while (read(fd, buffer, 1) > 0) {
        buffer[1] = '\0';
        text += string(buffer);
    }
    return (text);
}

void	File::closeFile(void) {
    close(_fd);
    _fd = -1;
}

// PRIVATE 

std::string File::get_file_with_path(string root, string name) {
    root = trim_line(root, "/");
    name = trim_line(name, "/");

    return (root + "/" + name);
}