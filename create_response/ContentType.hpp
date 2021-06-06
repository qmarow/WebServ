#ifndef CONTENTTYPE_HPP
#define CONTENTTYPE_HPP

#include <iostream>

class ContentType
{
public:
    typedef std::string         string;
    ContentType();
    ~ContentType();
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
