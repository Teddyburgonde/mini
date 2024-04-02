NAME=minishell

MY_SOURCES= src/prompt/minishell.c \
			src/prompt/ft_readline.c \
            src/env/init_env.c \
            src/env/print_env.c \
            src/signal/ft_ctrl.c \
			src/utils/minishell_utils.c \
			src/utils/minishell_utils_copy.c \
            src/utils/ft_putstr_fd.c \
            src/utils/lst_utils_env.c \
			src/utils/lst_utils_parse.c \
            src/utils/ft_strcmp.c \
			src/utils/ft_split.c \
			src/utils/utils_parsing.c \
			src/error_free/free_error.c \


MY_OBJECTS=$(MY_SOURCES:.c=.o)

red='\033[0;31m'
green='\033[0;32m'
orange='\033[0;33m'
blue='\033[0;34m'
purple='\033[0;35m'
cyan='\033[0;36m'
light_grey='\033[0;37m'
light_red='\033[1;31m'
light_green='\033[1;32m'
light_orange='\033[1;33m'
light_blue='\033[1;34m'
light_purple='\033[1;35m'
light_cyan='\033[1;36m'
white='\033[1;37m'
green_c='\033[6;32m'
RM=rm -f
CC=cc
FLAGS=-Wall -Werror -Wextra -g

all: $(NAME)

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(MY_OBJECTS)
	@clear
	@$(CC) $(FLAGS) $(MY_OBJECTS) -o $(NAME) -lreadline
	@echo $(light_cyan)"\t   »»-————　Make　————-««\n"


clean: 
	@echo $(light_green)"\t  »»-————　Clean　————-««\n"
	@$(RM) $(MY_OBJECTS) $(MY_TESTER)

fclean: clean
	@$(RM) $(NAME)
	@echo $(white)"\t»»-————　Full clean　————-««\n"

header:
	@echo $(white)"  ▄▄▄▄███▄▄▄▄    ▄█  ███▄▄▄▄    ▄█     ▄████████    ▄█    █▄       ▄████████  ▄█        ▄█      "
	@echo $(white)"▄██▀▀▀███▀▀▀██▄ ███  ███▀▀▀██▄ ███    ███    ███   ███    ███     ███    ███ ███       ███      "
	@echo $(white)"███   ███   ███ ███▌ ███   ███ ███▌   ███    █▀    ███    ███     ███    █▀  ███       ███      "
	@echo $(white)"███   ███   ███ ███▌ ███   ███ ███▌   ███         ▄███▄▄▄▄███▄▄  ▄███▄▄▄     ███       ███      "
	@echo $(white)"███   ███   ███ ███▌ ███   ███ ███▌ ▀███████████ ▀▀███▀▀▀▀███▀  ▀▀███▀▀▀     ███       ███      "
	@echo $(white)"███   ███   ███ ███  ███   ███ ███           ███   ███    ███     ███    █▄  ███       ███      "
	@echo $(white)"███   ███   ███ ███  ███   ███ ███     ▄█    ███   ███    ███     ███    ███ ███▌    ▄ ███▌    ▄"
	@echo $(white)" ▀█   ███   █▀  █▀    ▀█   █▀  █▀    ▄████████▀    ███    █▀      ██████████ █████▄▄██ █████▄▄██"
	@echo $(white)"                                                                             ▀         ▀        "

re: fclean all
	@echo $(green_c)"\t   »»-————　Done　————-««"

.PHONY: all clean fclean header re
