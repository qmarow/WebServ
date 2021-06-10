#ifndef UTILSSTRING_HPP
#define UTILSSTRING_HPP

#include <string>
#include <vector>
#include <iostream>

typedef std::string         string;
typedef std::vector<string> vector_string;

string          trim_line(string line, string set);
string          overlay_symbol(string line, char symbol, char overlay_symbol);
string          erase_after_symbol(string line, char symbol);
vector_string   split_line(string line, string symbols);
int             find_word(vector_string words, string word);
size_t          ft_strlen(char *s);
int             find_in_headers(string str, string s);

#endif