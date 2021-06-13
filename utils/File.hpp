#ifndef FILE_HPP
#define FILE_HPP

#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include "./../utils/UtilsString.hpp"

class File {
public:
	typedef std::string			string;
	typedef std::vector<string>	vector_string;

private:
	string	_file;
	
public:
	File();
	File(const File &file);
	File &operator=(const File &file);
	~File();

	int				open_file(string root, string name);
	int				open_file(string root, vector_string shredded_path);
	int				open_file(string root, vector_string shredded_path, string name);
	int				open_file(vector_string shredded_path);
	int				create_file(string root, string name);
	int				create_file(vector_string shredded_path);
	int				append_file(string text);
	int				write_file(string text);
	string			read_file(void);
	static string	read_file(int fd);
	int				delete_file(void);

private:
    string  get_file_with_path(string root, string name);
};


#endif