COLORGREEN = \033[32m
COLORREED = \033[31m
COLORWHITE = \033[0m

NAME = webserv

SRCS =	./CGI/CGI.cpp \
		./create_response/Response.cpp \
		./create_response/ErrorPage.cpp \
		./create_response/ContentType.cpp \
		./create_response/Autoindex.cpp \
		./handler_connects/Client.cpp \
		./handler_connects/HandlerConnects.cpp \
		./parse_confige/ParseConfige.cpp \
		./parse_confige/Server.cpp \
		./parse_request/ParseRequest.cpp \
		./parse_request/Request.cpp \
		./parse_url/URL.cpp \
		./utils/TransferNumberSystem.cpp \
		./utils/UtilsString.cpp \
		./utils/UtilsTime.cpp \
		./utils/decode_base64.cpp \
		./utils/File.cpp \
		./main.cpp

OBJ = $(SRCS:.cpp=.o)

FLAGS = -Wall -Wextra -Werror -std=c++98

%.o: %.c
	@@clang++ -I -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	@clang++ $(FLAGS) $^ -o $@
	@echo "$(COLORGREEN)Assembled webserv!$(COLORWHITE)"

clean:
	@rm -f ./*.o
	@rm -f ./*/*.o
	@rm -f ./*/*/*.o
	@echo "$(COLORREED)Deleted the object files.$(COLORWHITE)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(COLORREED)Deleted all.$(COLORWHITE)"

re: fclean all

.PHONY: all start clean fclean re