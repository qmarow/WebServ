#ifndef UTILSTIME_HPP
#define UTILSTIME_HPP

#include "./../header.hpp"

typedef std::string string;

static string week_day[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static string month[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

string  get_time_last_modified(char const *name_file);
string  get_time();

#endif
