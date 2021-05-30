#ifndef CONTENT_TYPE_HPP
#define CONTENT_TYPE_HPP

#include <iostream>

class Content_type
{
public:
    typedef std::string         string;
    Content_type();
    ~Content_type();
private:
    static string     _type_text[6];
    static string     _type_audio[4];
    static string     _type_image[8];
    static string     _type_video[7];
    static string     _type_multipart[4];
    static string     _type_application[14];
public:
    string      get_type(string expansion);
private:
    string      search_type(string expansion, string *types);
};

#endif
