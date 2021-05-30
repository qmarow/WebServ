#ifndef FILE_HPP
#define FILE_HPP

#include "./../header.hpp"

class File {
public:
	typedef std::string string;

private:
	int _fd;
	
public:
	File();
	File(const File &file);
	File &operator=(const File &file);
	~File();

	int		openFile(string root, string name);
	string	readFile(void);
	static string  readFile(int fd);
	void	closeFile(void);

private:
    string  get_file_with_path(string root, string name);
};


#endif