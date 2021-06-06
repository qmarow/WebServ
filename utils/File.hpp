#ifndef FILE_HPP
#define FILE_HPP

#include <fcntl.h>
#include <string>
#include <unistd.h>
#include "./../utils/UtilsString.hpp"

class File {
public:
	typedef std::string			string;
	typedef std::vector<string>	vector_string;

private:
	int _fd;
	string	_root;
	string	_name;
	
public:
	File();
	File(const File &file);
	File &operator=(const File &file);
	~File();

	// int				create_file(string root, string name);
	int				open_file(string root, string name);
	int				open_file(vector_string shredded_path);
	// int				append_file(string text);
	// int				write_file(string text);
	string			read_file(void);
	static string	read_file(int fd);
	int				delete_file(void);
	void			close_file(void);

private:
    string  get_file_with_path(string root, string name);
};


#endif