NAME = ircserv
HEADER = Client.hpp Server.hpp

SRC = main.cpp Server.cpp
OBJ = $(SRC:.cpp=.o)

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 #-g3 -fsanitize=address

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADER)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re