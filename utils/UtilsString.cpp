#include "UtilsString.hpp"

size_t  ft_strlen(char *s) {
    size_t size = 0;
    for (; size + s != 0; size++){}
    return (size);
}

int     find_in_headers(string str, string s) {
    int i = 0;
    int a = 0;

    while (i < str.size() && !(str[i] == '\n' && str[i + 1] == '\n')) {
        if (str[i + a] == s[a]) {
            for (; str[i + a] == s[a] && str[i + a] != 0 && s[a] != 0; ++a) {
            }
            if (a == s.size() ) {
                return (i);
            }
            a = 0;
        }
        ++i;
    }
    return (-1);
}

string trim_line(string line, string set) {
    int count_left = 0;
    int count_right = 0;

    for (int i = 0; i < line.size(); i++) {
        if (set.find(line[count_left]) != std::string::npos) {
            count_left++;
        }
        if (set.find(line[line.size() - 1 - count_right]) != std::string::npos) {
            count_right++;
        }
    }
    if ((count_left + count_right) >= line.size()) {
        return ("");
    }
    count_right = (line.size() - count_right) - count_left;
    return (line.substr(count_left, count_right));
}

string overlay_symbol(string line, char symbol, char overlay_symbol) {
    bool flag_quotation_mark = false;

    for (int i = 0; i < line.size(); i++) {
        if (line[i] == '\"' || line[i] == '\'') {
            flag_quotation_mark = !flag_quotation_mark;
        }
        if (line[i] == symbol && flag_quotation_mark == false) {
            line.insert(i, 1, overlay_symbol);
            line.insert(i + 2, 1, overlay_symbol);
            i += 2;
        }
    }
    return (line);
}

string erase_after_symbol(string line, char symbol) {
    bool flag_quotation_mark = false;

    for (int i = 0; i < line.size(); i++) {
        if (line[i] == '\'' || line[i] == '\"') {
            flag_quotation_mark = !flag_quotation_mark;
        }
        if (line[i] == symbol && flag_quotation_mark == false) {
            return (line.substr(0, i));
        }
    }
    return (line);
}

vector_string split_line(string line, string symbols) {
    vector_string words;
    int index_start = 0;
    int i = 0;
    bool flag_quotation_mark = false;

    line = trim_line(line, symbols);
    for (; i < line.size(); i++) {
        if (line[i] == '\"' || line[i] == '\'') {
            flag_quotation_mark = !flag_quotation_mark;
        }
        if (symbols.find(line[i]) != std::string::npos && flag_quotation_mark == false) {
            string new_line = line.substr(index_start, i - index_start);
            if (new_line.size() > 0) {
                words.push_back(line.substr(index_start, i - index_start));
            }
            while (symbols.find(line[i]) != std::string::npos) {
                i++;
            }
            if (i == line.size()) {
                return (words);
            }
            index_start = i--;
        }
    }
    string new_line = line.substr(index_start, 10000);
    if (new_line.size() > 0) {
        words.push_back(new_line);
    }
    return (words);
}

int find_word(vector_string words, string word) {
    for (int i = 0; i < words.size(); i++) {
        if (word == words[i]) {
            return (i);
        }
    }
    return (-1);
}