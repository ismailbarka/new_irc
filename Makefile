RESET_COLOR = \033[0m
RED_COLOR = \033[0;31m
GREEN_COLOR = \033[0;32m
YELLOW_COLOR = \033[0;33m
BLUE_COLOR = \033[0;34m
CYAN_COLOR = \033[0;36m

NAME = ircserv
HEADER = Client.hpp Server.hpp Channels.hpp

SRC =   Channels.cpp Client.cpp Server.InviteComm.cpp Server.JoinComm.cpp Server.KickComm.cpp Server.ListComm.cpp\
		Server.ModeComm.cpp Server.NickComm.cpp Server.PrivMsgComm.cpp Server.QuitComm.cpp Server.TopicComm.cpp\
		Server.UserComm.cpp Server.cpp Server.PartComm.cpp main.cpp Server.BotComm.cpp Server.HandleComm.cpp Server.PassComm.cpp
OBJ = $(SRC:.cpp=.o)

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 #-g3 -fsanitize=address

# Count the number of source files for progress indication
TOTAL_FILES := $(words $(SRC))
PROGRESS = 0

# Count the number of source files for progress indication
TOTAL_FILES := $(words $(SRC))
PROGRESS = 0

all: $(NAME)

$(NAME): $(OBJ)
	@echo ""
	@echo "Building $(BLUE_COLOR)$(NAME)...$(RESET_COLOR)"
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN_COLOR)Build complete.$(RESET_COLOR)"

%.o: %.cpp $(HEADER)
	@$(CC) $(FLAGS) -c $< -o $@
	@$(eval PROGRESS := $(shell echo $$(($(PROGRESS)+1))))
	@$(eval PERCENT := $(shell echo $$(($(PROGRESS)*100/$(TOTAL_FILES)))))
	@printf "[%d/%d] (%.2f%%) " $(PROGRESS) $(TOTAL_FILES) $(PERCENT)
	@$(eval BLOCKS := $(shell echo $$((($(PROGRESS)*20)/$(TOTAL_FILES)))))
	@printf "$(YELLOW_COLOR)%0.s█$(RESET_COLOR)" $$(seq 1 $(BLOCKS))
	@printf "$(YELLOW_COLOR)%0.s $(RESET_COLOR)" $$(seq 1 $$(echo "20-$(BLOCKS)" | bc))
	@printf "\r"  # Carriage return to overwrite the same line
clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
