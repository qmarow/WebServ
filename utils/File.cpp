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

// int     File::create_file(string root, string name) {
//     string file;

//     file = get_file_with_path(root, name);
//     _fd = open(file.c_str(), O_CREAT | O_RDWR, ~0);
//     if (_fd == -1) {
//         return (1);
//     }
//     return (0);
// }

int		File::open_file(string root, string name) {
    string file;

    _root = root;
    _name = name;
    file = get_file_with_path(root, name);
    _fd = open(file.c_str(), O_RDONLY);
    if (_fd == -1) {
        return (1);
    }
    return (0);
}

int     File::open_file(vector_string shredded_path) {
    string root;
    string name;

    for (int i = 0; i < shredded_path.size() - 1; i++) {
        root += shredded_path[i];
        root += (i + 2 < shredded_path.size()) ? "/" : "";
    }
    name = shredded_path[shredded_path.size() - 1];
    return (open_file(root, name));
}

// int     File::append_file(string text) {
//     string  old_text;
//     int     code_error;

//     old_text = read_file(_fd);
//     std::cout << "old: " << old_text << "\n";
//     // old_text += text;
//     code_error = write(_fd, old_text.c_str(), old_text.size());
//     if (code_error == -1) {
//         return (1);
//     }
//     return (0);
// }

// int     File::write_file(string text) {
//     int code_error;

//     code_error = write(_fd, text.c_str(), text.size());
//     if (code_error == -1) {
//         return (1);
//     }
//     return (0);
// }

std::string	File::read_file(void) {
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

std::string	File::read_file(int fd) {
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

void	File::close_file(void) {
    close(_fd);
    _fd = -1;
}

int     File::delete_file(void) {
    int     pid;
    int     status;
	string  path;
	char	*buf[3];

	path = get_file_with_path(_root, _name);
	buf[0] = strdup("/bin/rm");
	buf[1] = strdup(path.c_str());
	buf[2] = NULL;

    if ((pid = fork()) == 0) {
        execve("/bin/rm", buf, NULL);
        return (1);
    } else {
        waitpid(pid, &status, 0);
        free(buf[0]);
        free(buf[1]);
        free(buf[2]);
    }
    return (0);
}

// PRIVATE 

std::string File::get_file_with_path(string root, string name) {
    root = trim_line(root, "/");
    name = trim_line(name, "/");

    return (root + "/" + name);
}