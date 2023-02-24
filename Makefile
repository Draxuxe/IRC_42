# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aleferra <aleferra@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/20 14:07:04 by aleferra          #+#    #+#              #
#    Updated: 2023/02/23 16:46:38 by aleferra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

BOT = ircbot

# Color
FG_MAGE	= \033[0;35m
FG_CYAN	= \033[0;36m
FG_WHIT	= \033[0;37m
FG_GREE	= \033[0;32m

INC_DIR = includes/
SRC_DIR = srcs/
OBJ_DIR = objs/

INC_BOT = bot_include/
SRC_BOT = bot/
OBJ_BOT = objs_bot/

# Includes
_INCS = serv.hpp \
		user.hpp \
		nc.hpp \
		message.hpp \
		signal.hpp \
		color.hpp \
		channel.hpp \
		cmds.hpp

# Sources
_SRCS =	main.cpp \
		serv/serv.cpp \
		clients/user.cpp \
		clients/nc.cpp \
		channels/message.cpp \
		channels/channel.cpp \
		commands/cmds.cpp \
		commands/join.cpp \
		commands/kick.cpp \
		commands/mode.cpp \
		commands/msg.cpp \
		commands/nick.cpp \
		commands/part.cpp \
		commands/ping.cpp \
		commands/quit.cpp

_INCS_BOT = bot.hpp 

_SRCS_BOT =	bot.cpp 

SRCS = $(addprefix $(SRC_DIR), $(_SRCS))
OBJS = $(addprefix $(OBJ_DIR), $(_SRCS:.cpp=.o))
INCS = $(addprefix $(INC_DIR), $(_INCS))

SRCS_BOT = $(addprefix $(SRC_BOT), $(_SRCS_BOT))
OBJS_BOT = $(addprefix $(OBJ_BOT), $(_SRCS_BOT:.cpp=.o))
INCS_BOT = $(addprefix $(INC_BOT), $(_INCS_BOT))
# Compile
CC = c++

# Flags
CFLAGS = -Wall -Werror -Werror -std=c++98
INCLUDE = -I $(INC_DIR)
INCLUDE_BOT = -I $(INC_BOT)

# Rules
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/serv
	@mkdir -p $(OBJ_DIR)/commands
	@mkdir -p $(OBJ_DIR)/clients
	@mkdir -p $(OBJ_DIR)/channels
	@mkdir -p $(OBJ_DIR)/utils
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(INCS) $(OBJS) Makefile
	@echo "Number of objs files: [$(words $(OBJS))]"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "[FT_IRC]"
	@echo "$(FG_GREE)Ready to launch '$(NAME)'.$(FG_WHIT)"

$(OBJ_BOT)%.o: $(SRC_BOT)%.cpp
	@mkdir -p $(OBJ_BOT)
	@$(CC) $(CFLAGS) $(INCLUDE_BOT) -c $< -o $@

$(BOT): $(INCS_BOT) $(OBJS_BOT) Makefile
	@$(CC) $(CFLAGS) $(OBJS_BOT) -o $(BOT)
	@echo "[FT_IRC]"
	@echo "$(FG_GREE)Ready to launch '$(BOT)'.$(FG_WHIT)"

all: $(NAME)

bot: $(NAME) $(BOT)

clean:
	@rm -f $(OBJS) $(NAME)
	@rm -rf $(OBJ_DIR)
	@echo "[FT_IRC]"
	@echo "$(FG_MAGE)Objects files cleaned.$(FG_WHIT)"

fclean: clean
	@rm -rf $(NAME) $(OBJS)
	@rm -rf $(BOT) $(OBJS_BOT) $(OBJ_BOT)
	@echo "$(FG_MAGE)Program files cleaned.$(FG_WHIT)"

re: fclean all

.PHONY: all clean fclean re