#include "UtilsTime.hpp"

std::string  get_time_last_modified(char const *name_file)
{
    FILE *fp;
    struct stat buff;
    if((fp=fopen(name_file, "rb"))==NULL) {
        printf("Cannot open file.\n");
        exit (1);
    }
    stat(name_file, &buff);
    struct timespec t = buff.st_ctimespec;
    struct tm* now = localtime(&t.tv_sec);
    std::stringstream  time;

    time << week_day[now->tm_wday] << ", " << now->tm_mday
         << " " << month[now->tm_mon] << " " <<  (now->tm_year + 1900) << " "
         <<  now->tm_hour << ":" <<  now->tm_min << ":" <<  now->tm_sec
         << " GMT";
    return (time.str());
}

std::string  get_time()
{
    time_t t = time(0);   // get time now
    tm* now = localtime(&t);
    std::stringstream  time;
    time << week_day[now->tm_wday] << ", " << now->tm_mday
         << " " << month[now->tm_mon] << " " <<  (now->tm_year + 1900) << " "
         <<  now->tm_hour << ":" <<  now->tm_min << ":" <<  now->tm_sec
         << " GMT";
    return (time.str());
}