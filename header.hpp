#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sstream>
#include <sys/time.h>
#include <sys/stat.h>
#include "./parse_confige/ParseConfige.hpp"
#include "./parse_confige/Server.hpp"
#include "./parse_request/ParseRequest.hpp"
#include "./parse_request/Request.hpp"
#include "./handler_connects/HandlerConnects.hpp"
#include "./handler_connects/Client.hpp"
#include "./utils/UtilsString.hpp"
#include "./utils/UtilsTime.hpp"
#include "./utils/File.hpp"
#include "./parse_url/URL.hpp"
#include "./create_response/Content_type.hpp"
#include "./create_response/Response.hpp"

#endif