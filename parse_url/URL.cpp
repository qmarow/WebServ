#include "URL.hpp"

// PUBLIC

URL::URL() {}

URL::URL(const URL &url) {
    *this = url;
}

URL &URL::operator=(const URL &url) {
    _path = url._path;
    _keys_argv = url._keys_argv;
    _values_argv = url._values_argv;
    return (*this);
}

URL::~URL() {}

void URL::parse(string url_string) {
    _path = "/";
    _keys_argv = vector_string();
    _values_argv = vector_string();

    std::cout << "parce 1\n";
    vector_string data = split_line(url_string, "?#");
    if (data.size() != 0) {
        _path = data[0];
    }
    std::cout << "parce 2\n";
    for (int i = 1; i < data.size(); i++) {
        vector_string key_and_value = split_line(data[i], "=");
        _keys_argv.push_back(key_and_value[0]);
        _values_argv.push_back(key_and_value[1]);
    }
    _path = trim_line(_path, "/");
}

string URL::get_path(void) {
    return (_path);
}

int URL::count_argv(void) {
    return (_keys_argv.size());
}

vector_string URL::get_keys_argv(void) {
    return (_keys_argv);
}

vector_string URL::get_values_argv(void) {
    return (_values_argv);
}

string URL::at_keys(int index) {
    return (_keys_argv[index]);
}

string URL::at_values(int index) {
    return (_values_argv[index]);
}