NAME=minishell

MY_SOURCES= src/prompt/minishell.c \
			src/prompt/ft_readline.c \
			src/prompt/setup_command.c \
			src/prompt/cmd_manager.c \
			src/prompt/manage_cmd_selector.c \
            src/env/init_env.c \
			src/env/env_to_char.c \
			src/env/update_shlvl.c  \
			src/env/env_command.c \
            src/signal/signal.c \
			src/builtin/export.c \
			src/builtin/ft_cd.c \
			src/builtin/ft_echo.c \
			src/builtin/ft_pwd.c \
			src/builtin/unset.c \
			src/builtin/ft_exit.c \
			src/parsing/parsing.c \
			src/parsing/parsing_expand.c \
			src/parsing/redirection_expand.c \
			src/exec/check.c \
			src/exec/redirection.c \
			src/exec/fork.c \
			src/exec/heredoc.c \
			src/exec/path.c \
			src/exec/flow_redirections.c \
			src/exec/build_path.c \
			src/utils/parse_command_utils.c \
			src/utils/minishell_utils.c \
			src/utils/minishell_utils_copy.c \
            src/utils/ft_putstr_fd.c \
			src/utils/lstlast_utils.c \
			src/utils/lst_new_utils.c \
			src/utils/lst_new_utils_bis.c \
            src/utils/lst_addback_utils.c \
			src/utils/lst_addback_utils_bis.c \
			src/utils/lst_utils_env.c \
			src/utils/lst_utils.c \
            src/utils/ft_strcmp.c \
			src/utils/ft_split.c \
			src/utils/parsing_utils.c \
			src/utils/parsing_redirection_utils.c \
			src/utils/skip.c \
			src/utils/len_utils.c \
			src/utils/ft_strncmp.c \
			src/utils/ft_strlcpy.c \
			src/utils/builtins_utils.c \
			src/utils/builtins_utils_two.c \
			src/utils/error_message_for_exit.c \
			src/utils/fork_processes_utils.c \
			src/utils/process_utils.c \
			src/utils/exit_status.c \
			src/utils/copy_utils.c \
			src/utils/lst_clear_utils.c \
			src/utils/echo_utils.c \
			src/error_free/free_error.c \
			src/utils/ft_strjoin_file_utils.c \
			src/utils/ft_strjoin_arg_utils.c \
			src/utils/ft_strjoin_until_utils.c \
			src/utils/ft_strjoin_quoted_arg_utils.c  \
			src/utils/verif_command_line_utils.c \
			src/utils/skips_utils.c \
			src/utils/heredoc_utils.c \
			src/utils/redirections_utils.c \
			src/utils/ft_itoa.c \
			src/utils/setup_protect.c \
			src/utils/expand_heredoc.c \
			src/utils/strlen_heredoc.c \
			src/utils/little_utils.c \
			src/utils/close_fd.c \
			src/utils/close_pipe.c \
			src/utils/env_utils.c \
			src/utils/ft_cmd_manager_utils.c  \
			src/utils/env_arg_utils.c \
			src/utils/parsing_command_utils.c \
			src/utils/parsing_initialize.c \
			src/utils/parsing_utils_two.c \
			src/utils/fill_command_paths_utils.c


MY_OBJECTS=$(MY_SOURCES:.c=.o)

white='\033[1;37m'

RM=rm -f
CC=cc
FLAGS=-Wall -Werror -Wextra -g

MAKEFLAGS += --no-print-directory

all: $(NAME)

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(MY_OBJECTS)
	@clear
	@$(CC) $(FLAGS) $(MY_OBJECTS) -o $(NAME) -lreadline
	@echo $(light_cyan)"\t   »»-————　Make　————-««\n"
	# @make header


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
