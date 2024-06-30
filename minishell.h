/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 11:05:59 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/30 15:41:55 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <limits.h>
# include <string.h>

typedef enum s_bool
{
	TRUE = 1,
	FALSE = 0
}	t_bool;

/*
* Global variables
*/

extern int	g_sig;

typedef struct s_vars
{
	pid_t	child;
	pid_t	last_child;
	int		nb_cmd;
	int		cmd_index;
	char	**path;
	char	***cmd;
	char	**full_cmd;
	int		pipe_1[2];
	int		pipe_2[2];
	char	**env;
	int		exit_code;
	int		exit_code_signal;
	int		*pids;
}	t_vars;

/*
* Environment
*/

typedef struct s_env
{
	char			*var_name;
	char			*value;
	char			*full_path;
	t_bool			hide;
	struct s_env	*next;
}	t_env;

/*
* Parsing
*/

typedef struct s_argument_to_expand
{
	char						*content;
	struct s_argument_to_expand	*next;
}		t_argument_to_expand;

typedef struct s_redirection_to_expand
{
	char							*arg;

	enum
	{
		REDIRECTION_OUTFILE,
		REDIRECTION_INFILE,
		REDIRECTION_APPEND,
		REDIRECTION_HEREDOC,
		UNASIGNED
	}	e_type;
	struct s_redirection_to_expand	*next;
}			t_redirection_to_expand;

typedef struct s_command_to_expand
{
	t_argument_to_expand		*arguments;
	t_redirection_to_expand		*redirections;

	struct s_command_to_expand	*next;
}	t_command_to_expand;

typedef struct command_line_parsing_result
{
	t_bool				did_succeed;

	t_command_to_expand	*commands;

}	t_command_line_parsing;

typedef struct command_parsing_result
{
	t_bool				did_succeed;

	t_command_to_expand	*command;

	const char			*remaining_line;

}	t_command_parsing_result;

typedef struct redirection_parsing
{
	t_bool					did_succeed;

	t_redirection_to_expand	*redirection;

	const char				*remaining_line;

}	t_redirection_parsing;

typedef struct argument_parsing
{
	t_bool					did_succeed;

	t_argument_to_expand	*argument;

	const char				*remaining_line;

}	t_argument_parsing;

/*
* Expand parsing 
*/

typedef struct s_char_list
{
	char				value;
	t_bool				last_pos;
	t_bool				was_in_a_variable;

	struct s_char_list	*next;
}	t_char_list;

typedef struct s_argument
{
	t_char_list			*chars;

	struct s_argument	*next;	
}	t_argument;

/*
* Expand redirections
*/

typedef struct s_redirection
{
	int						infile_fd;
	int						outfile_fd;
	int						nb_heredoc;
	char					*file_heredoc;
	char					*limiter;
	char					*name_infile;
	char					*name_outfile;	
	enum
	{
		HERE,
		COMING,
		NONE
	}	e_position;
	t_bool					ambiguous;
	struct s_redirection	*next;
}	t_redirection;

/*
* Utilitaries
*/

void							*ft_calloc(size_t nmemb, size_t size);
int								ft_strchr(char *s, int c);
void							ft_putstr_fd(char *s, int fd);
char							*copy(char *s);
char							**ft_split(char const *s, char c);
int								ft_strcmp(char *s1, char *s2);
int								ft_strlen(char const *str);
char							*ft_strjoin(char const *s1, char *s2);
char							*ft_strjoin_free_s2(char *s1, char *s2);
int								single_redirection(char *str);
int								double_redirection(char *str);
int								ft_strcspn(const char *s, char *reject);
int								skip_dolar_var(char *argument, int index);
int								ft_atoi(const char *str);
int								ft_lstsize_command(t_command_to_expand *cmd);
size_t							ft_strlcpy(char *dst,
									const char *src, size_t size);
int								ft_lstsize_env(t_env *env);
int								ft_strncmp(const char *s1,
									const char *s2, size_t n);
int								ft_strcspn2(const char *s, char *reject);
char							*ft_itoa(int n);
char							*copy_without_quote(char *s);
void							ft_exit_message_0(void);
void							ft_exit_message_2(char *command);
void							ft_exit_message_too_many_arguments(void);
void							ft_exit_message_argument_required(
									char *command);
int								ft_isdigit(int c);
void							process_commands(t_vars *vars,
									t_redirection **redirect, t_env **envp);
int								choice_pipe_setup(t_vars *vars);
int								setup_pipe(int *pipe_fd);
void							initialize_vars(t_vars *vars);
int								wait_process(t_vars *vars);
int								process(t_vars *vars,
									t_redirection *redirect, t_env **env);
void							handle_pipe_closing(t_vars *vars);
int								child_process(t_vars *vars,
									t_redirection *redirect,
									char **actual_cmd, t_env **env);
void							error_close_files(t_redirection *redirect);
void							ft_close_fd(t_vars *vars);
t_bool							is_builtins_exec(t_vars *vars);
t_bool							is_builtins_parsing(char **str);
void							copy_content(char *tab,
									const char *s1, int len);
char							*allocate_tab(const char *s1, int *len);
char							*allocate_and_prepare_tab(const char *s1,
									const char *s2);
void							copy_and_concatenante(char *tab,
									const char *s1, const char *s2);
char							*ft_allocate_tab(const char *s1,
									const char *s2, char *reject);
void							copy_tab(char *tab, const char *s1,
									const char *s2, char *reject);
int								skip_spaces_and_tabs(char *str);
int								check_spaces_and_tabs(char *command_line,
									t_command_line_parsing *parsing_result,
									t_vars *vars);
void							add_history_and_parse(char *command_line,
									t_command_line_parsing **parsing_result);
int								handle_parsing_errors(
									t_command_line_parsing *parsing_result,
									char *command_line, t_vars *vars);
int								check_pipe_position(char *command_line,
									t_vars *vars);
int								check_empty_quotes(char *command_line,
									t_command_line_parsing *parsing_result,
									t_vars *vars);
int								check_error_redirect_infifle_fd(
									t_redirection *redirect);
int								check_error_redirect_outfile_fd(
									t_redirection *redirect);
void							close_pipe_odd(t_vars *vars);
void							close_pipe_even(t_vars *vars);
char							**sort_env(char **env);
void							add_quote_after_equal(char *str, char *result);
char							*setup_full_path(char *str);
char							**env_to_char_export(t_env *env);
int								count_hidden_var(t_env *env);
void							cleanup_vars_path(t_vars *vars);
void							manage_variable_path(
									t_env **env, t_vars *vars);
void							print_env_vars(
									t_vars *vars, t_redirection *redirect);
int								verif_arg_env(char **command_line);
int								pipes_management(char **remaining_line);
int								verif_command_parsing(
									t_command_parsing_result *command_parsing);
t_redirection_parsing			*initialize_redirection_parsing_result(void);
int								check_remaining_line_length(
									char *remaining_line,
									t_command_parsing_result *result);
void							initialize_allocated_parse_command(
									t_command_parsing_result **result,
									t_redirection_parsing **redirection_result,
									t_argument_parsing **argument_result);
t_command_parsing_result		*ft_redirections_arguments(
									char **remaining_line,
									t_command_parsing_result *result,
									t_redirection_parsing *redirection_result,
									t_argument_parsing *argument_result);
t_argument_parsing				*parse_quote(const char *remaining_line,
									t_argument_parsing *result);
t_command_parsing_result		*ft_redirections_arguments(
									char **remaining_line,
									t_command_parsing_result *result,
									t_redirection_parsing *redirection_result,
									t_argument_parsing *argument_result);
t_redirection_parsing			*ft_verif_redirection(char *str,
									t_redirection_parsing *redirection_result);
t_redirection_parsing			*parse_redirection(char *str);
int								fill_command_paths(t_vars *vars,
									t_command_to_expand *tmp, t_env *env);
int								process_command_path(t_vars *vars,
									t_command_to_expand *tmp,
									t_env *env, int i);
int								copy_command_line(char **command_line,
									t_vars *vars,
									int i, t_argument *final_parsing);
int								handle_command_line(t_vars *vars,
									char **command_line,
									t_argument *final_parsing, int i);
char							**copy_double_array(char **src);
size_t							ft_array_len(char **arr);

void							strlen_expand(char *var_name,
									t_env *env, int *count, int *j);
void							strlen_exit_code(t_vars *vars, int *count);
int								expand_len(t_redirection_to_expand *tmp,
									t_vars *vars, t_env *env, int i);
int								ft_strlen_ultime(t_redirection_to_expand *tmp,
									t_env *env, t_vars *vars);
char							*ft_remove_simple_quote(char *src);
int								expand_redirection(
									t_redirection_to_expand **tmp,
									t_vars *vars, t_env *env,
									t_redirection_to_expand *redirect);
t_redirection_to_expand			*get_redirection_list(
									t_redirection_to_expand *redirect,
									t_env *env, t_vars *vars);
t_bool							is_in_quote(char *str, int i);
void							refresh_quotes_status(t_bool *in_quote,
									char current);
char							*ft_substr(char const *s,
									unsigned int start, size_t len);
char							*make_var(char *str);
int								verif_export(char *str);
void							update_env_var(t_env *tmp_env,
									char *cmd, char *value);
void							hide_and_update_env_var(t_env *tmp_env,
									char *cmd);	

/*
* Env
*/

void							init_env(t_env **env, char **envp);
char							**env_to_char(t_env *env);
char							**env_to_char_export(t_env *env);
char							*update_shlvl(int shlvl);
char							*convert_to_string(int shlvl, int len);
int								calculate_length(int shlvl);
int								adjust_shlvl(int shlvl);
void							add_modified_env_var(t_env **env, char *cmd,
									char *var_name, char *value);
void							add_new_env_var(t_env **env, char *cmd,
									char *var_name, char *value, int hide);
char							*remove_plus(char *str);

/*
* Signal
*/

void							ft_ctrl_c(int signal);
void							ctrl_d(int signal);
void							run_signals(void);

/*
* Prompt
*/

int								ft_readline(t_env **env, t_vars *vars);
int								manage_echo_pwd(
									char **command_line,
									t_redirection *redirect, t_vars **vars);
int								manage_cd_unset(char **command_line,
									t_vars **vars, t_env **env);
int								manage_export(char **command_line,
									t_redirection *redirect,
									t_vars **vars, t_env **env);
int								manage_env(char **command_line,
									t_vars **vars, t_redirection *redirect);
int								manage_exit(char **command_line, t_vars **vars);
int								cmd_selector(t_env **env,
									char **command_line, t_vars *vars,
									t_redirection *redirect);
void							print_env(char **env, t_vars *vars,
									t_redirection *redirect);
void							exec_env(char **command_line,
									t_vars *vars, t_redirection *redirect);

/*
* Parsing
*/

char							*skip_spaces(const char *str);
char							*skip_one_character(const char *str);
char							*skip_quote(const char *str,
									char c, t_argument_parsing *result);
char							*ft_skip_arg(const char *str, char *reject);
char							*ft_strjoin_until(char *s1,
									const char *s2, char *reject);
char							*ft_strjoin_arg(char *s1, const char *s2);
char							*ft_strjoin_file(char *s1);
char							*allocate_tab_quoted(const char *s1,
									const char *s2, char *reject);
char							*ft_strjoin_quoted_arg(char *s1,
									const char *s2, char *reject);
void							copy_and_concatenate_quoted_arg(char *tab,
									const char *s1,
									const char *s2, char *reject);

t_command_line_parsing			*ft_parse_command_line(char *command_line);
t_command_parsing_result		*ft_allocated_result(void);
t_command_parsing_result		*redirections(t_command_parsing_result *result,
									char *remaining_line,
									t_redirection_parsing *redirection_result);
t_command_parsing_result		*arguments(t_command_parsing_result *result,
									t_argument_parsing *argument_result,
									char *remaining_line);
t_command_parsing_result		*parse_command(char *command_line);
t_command_parsing_result		*ft_redirections_arguments(
									char **remaining_line,
									t_command_parsing_result *result,
									t_redirection_parsing *redirection_result,
									t_argument_parsing *argument_result);
t_argument_parsing				*is_parsing_arg(const char *remaining_line,
									t_argument_parsing *result);
t_argument_parsing				*parse_quote(const char *remaining_line,
									t_argument_parsing *result);
t_redirection_parsing			*parse_redirection(char *str);
t_argument						*ft_expand_argument(
									const t_argument_to_expand	*argument,
									t_env *env, t_vars *vars);
t_argument						*ft_expand_vars_in_argument(
									const char *argument,
									t_env *env, t_vars *vars);
int								expand_argument(char *argument,
									t_env *env, t_vars *vars,
									t_char_list **chars);
t_bool							init_function(char *quote, t_argument **tmp,
									t_argument	**splitted_arguments,
									t_argument *argument_to_split);
int								function_verif_quote(t_char_list **tmp_char,
									char *quote,
									t_bool *quote_in_var);
void							fill_no_quote_arg(t_char_list **tmp_char,
									t_char_list **splitted_chars, char quote);
t_bool							quote_function(t_char_list **tmp_char,
									char *quote, t_bool in_quote);
int								ft_split_argument(t_argument *argument_to_split,
									t_argument **args);
t_bool							need_to_increment(char *argument, int i);

/*
* Setup command
*/

char							**ft_setup_command(t_argument *arg);

/*
* Command manager
*/

int								ft_cmd_manager(t_env **env,
									t_command_line_parsing *cmd,
									t_vars *vars);
int								cmd_selector(t_env **env,
									char **command_line,
									t_vars *vars, t_redirection *redirect);

/*
* Builtins
*/

int								export(t_env **env, char **cmd);
int								ft_cd(char **command, t_env **env);
int								ft_echo(char **command,
									t_vars *vars, t_redirection *redirect);
int								ft_pwd(t_vars *vars, t_redirection *redirect);
int								unset(t_env **env, char **names);
int								ft_exit(char **command);
int								is_there_an_option_n(char **command);
void							print_with_option_n(char **command);
void							print_not_option_n(char **command);
void							print_with_option_n_fd(char **command, int fd);
void							print_not_option_n_fd(char **command, int fd);
int								has_invalid_argument(char *arg, char *next_arg);

/*
* Chain list
*/

t_env							*ft_lstnew_env(void);
t_env							*lst_search_env(char *s, t_env *env);
t_argument_to_expand			*lst_new_argument_parsing_result(void);
t_command_to_expand				*lst_new_command_parsing_result(void);
t_redirection_to_expand			*lst_new_redirection_parsing_result(void);
t_char_list						*lst_new_char_list(void);
t_argument						*lst_new_argument(void);
void							ft_lstclear_env(t_env **lst);
void							ft_lstclear_commands(t_command_to_expand **lst);
void							ft_lstclear_arguments(
									t_argument_to_expand **lst);
void							ft_lstclear_redirections(
									t_redirection_to_expand **lst);
void							ft_lstadd_back_env(t_env **lst, t_env *new);
void							ft_command_to_expand_addback(
									t_command_to_expand **lst,
									t_command_to_expand *new);
void							ft_redirection_to_expand_addback(
									t_redirection_to_expand **lst,
									t_redirection_to_expand *new);
void							ft_argument_to_expand_addback(
									t_argument_to_expand **lst,
									t_argument_to_expand *new);
void							ft_lstadd_back_argument(
									t_argument **lst, t_argument *new);
void							ft_lstadd_back_char_list(
									t_char_list **lst, t_char_list *new);
void							ft_lstadd_back_redirection(
									t_redirection **lst, t_redirection *new);
t_redirection					*ft_lstnew_redirection(void);
void							ft_lstclear_argument(t_argument **lst);
void							ft_lstclear_char_list(t_char_list **lst);

/*
* Free / Error
*/

void							ft_free(char **tab);
void							ft_free_vars_input(
									char *command_line, char **env);
void							ft_free_tab_3d(t_vars *vars);
void							ft_lstclear_final_redirection(
									t_redirection **lst);

/*
* Exit_status
*/
void							wait_pids(t_vars *vars);
void							set_interactive_mode(int set);

/*
* Exec
*/

void							ft_heredoc(t_redirection *redirection,
									t_redirection_to_expand *all,
									t_env *env, t_vars *vars);
int								open_files(t_vars *vars,
									t_redirection_to_expand *redir);
char							**find_the_accessible_path(char **path,
									t_vars *vars, char **command_line);
int								build_path(char **path,
									char **bin_path,
									char **is_valid_cmd, char **full_cmd);
void							update_full_cmd(char ***full_cmd,
									char *is_valid_cmd);
int								verif_fill_command_paths(
									t_vars *vars, t_command_to_expand *tmp,
									t_env *env);
int								fork_processes(t_vars *vars,
									t_redirection **redirect, t_env **envp);
void							capture_and_redirection(
									char *tab, char *tmp, t_vars *vars);
void							open_fd_infile(t_vars *vars);
void							open_hd_w(t_vars *vars);
void							pipe_command_redirection_even(
									t_redirection *redirect, t_vars *vars);
void							pipe_command_redirection_odd(
									t_redirection *redirect, t_vars *vars);
void							last_command_redirection_odd(
									t_redirection *redirect, t_vars *vars);
void							first_command_redirection(t_vars *vars,
									t_redirection *redirect);
void							last_command_redirection_even(
									t_redirection *redirect, t_vars *vars);
void							ft_flow_redirection(t_vars *vars,
									t_redirection *redirect);
char							*get_var_name(char *str);
t_bool							is_builtins_exec(t_vars *vars);
t_bool							is_builtins_parsing(char **str);
t_redirection_to_expand			*is_last(t_redirection_to_expand *tmp);
void							stock_redirection(t_command_to_expand *list,
									t_env *env, t_vars *vars,
									t_redirection **redirection);
char							*ft_strjoin_mod(char *s1, char *s2);
t_redirection					*ft_lstlast_redirection(t_redirection *lst);
t_redirection_to_expand			*get_redirection_list(
									t_redirection_to_expand *redirect,
									t_env *env, t_vars *vars);
t_redirection_to_expand			*ft_expand_redirections(
									t_redirection_to_expand **redirection,
									t_env *env, t_vars *vars);
void							where_are_heredoc(t_redirection **list,
									t_bool exist);
void							prepare_heredoc(t_redirection *redirection,
									t_redirection_to_expand *tmp_redirection);
void							heredoc_setup(t_redirection *redirection,
									t_command_to_expand *tmp_command,
									t_env *env, t_vars *vars);
void							complete_heredoc(t_redirection *redirection,
									t_command_to_expand *tmp_command,
									t_env *env, t_vars *vars);
void							outfile_setup(t_redirection *redirection,
									t_redirection_to_expand *tmp_redirection);
void							infile_setup(t_redirection *redirection,
									t_redirection_to_expand *tmp_redirection);
void							append_setup(t_redirection *redirection,
									t_redirection_to_expand *tmp_redirection);
t_redirection					*no_redirection(void);
t_bool							global_redirections(t_redirection *redirection,
									t_redirection_to_expand *tmp_redirection,
									t_vars *vars);
char							*ft_strdup(char *s);
void							init(t_bool	*heredoc, int *nb_hd,
									t_redirection **result,
									t_redirection **redirection);
void							redirection_setup(t_redirection **redirection,
									int nb_hd);
void							security_clear(t_command_to_expand **list);
t_bool							is_last_infile(
									t_redirection_to_expand *tmp_redirection);
int								ft_strlen_with_expand(char *str, t_env *env,
									t_vars *vars);
char							*expand_line(char *str, t_env *env,
									t_vars *vars);
void							increment_index(int *i, int *t, char *tmp);
void							init_expand(int *i, int *j, int *t,
									char **var_name);
void							refresh_index(char *str, int *i,
									int *count, int j);
t_bool							need_to_be_expand_hd(char *str, t_env *env);
int								ft_strlen_with_expand(char *str, t_env *env,
									t_vars *vars);

#endif
