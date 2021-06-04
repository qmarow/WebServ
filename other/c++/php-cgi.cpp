#include <unistd.h>
#include <iostream>
#include <string>
#include <fcntl.h>

void    print_error(void);

int main(int argc, char **argv, char **env) {
    char *buf[3];
    int     pid;
    int     status;

    if (argc == 1) {
        print_error();
        return (-1);
    }
    buf[0] = strdup("/usr/bin/php");
    buf[1] = strdup(argv[1]);
    buf[2] = NULL;
    if ((pid = fork()) == 0) {
        if (execve(buf[0], buf, env) == -1) {
            free(buf[0]);
            free(buf[1]);
            print_error();
            return (-1);
        }
    } else {
        waitpid(pid, &status, 0);
        free(buf[0]);
        free(buf[1]);
    }
    return (0);
}

void print_error(void) {
    std::string error;

    error += "<html>\n";
    error +=    "\t<head>\n";
    error +=        "\t\t<title>php script missing</title>\n";
    error +=    "\t\t<body>\n";
    error +=        "\t<h1>php script missing</h1>\n";
    error +=    "\t</body>\n";
    error += "</html>\n";
    std::cout << error << "\n";
}
