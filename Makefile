NAME = irc
CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
SRC = nick.cpp pass.cpp user.cpp

SRCH = channel.hpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ) $(SRCH)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean $(NAME)