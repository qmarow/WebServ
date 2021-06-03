#ifndef UTILSTIME_HPP
#define UTILSTIME_HPP

#include <string>
#include <sstream>
#include <sys/time.h>
#include <sys/stat.h>

static std::string week_day[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static std::string month[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

std::string  get_time_last_modified(char const *name_file);
std::string  get_time();

#endif
