#include "File.hpp"

// PUBLIC

File::File() {
    _file = string();
}

File::File(const File &file) {
    *this = file;
}

File &File::operator=(const File &file) {
    _file = file._file;
    return (*this);
}

File::~File() {}

int		File::open_file(string root, string name) {
    int     fd;

    _file = get_file_with_path(root, name);
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

int     File::append_file(string text) {
    string  old_text;
    int     code_error;

    old_text = read_file();
    old_text += text;
    code_error = write_file(old_text);
    if (code_error == -1) {
        return (1);
    }
    return (0);
}

int     File::write_file(string text) {
    int code_error;
    int fd;

    fd = open(_file.c_str(), O_CREAT | O_TRUNC | O_WRONLY, ~0);
    if (fd == -1) {
        return (1);
    }
    code_error = write(fd, text.c_str(), text.size());
    if (code_error == -1) {
        return (2);
    }
    close(fd);
    return (0);
}

std::string	File::read_file(void) {
    string	text;
    int     fd;

    fd = open(_file.c_str(), O_RDONLY);
    text = read_file(fd);
    close(fd);
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

int     File::delete_file(void) {
    int     pid;
    int     status;
	string  path;
	char	*buf[3];

	buf[0] = strdup("/bin/rm");
	buf[1] = strdup(_file.c_str());
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