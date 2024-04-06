/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 11:05:59 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/06 15:08:21 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define SPACE 32
# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# define TAB '\t'
# define NEW_LINE '\n'
# define TRUE 1
# define FALSE 0



typedef struct s_vars{
	char	*input;
}	t_vars;

typedef struct s_env{
	char			*var;
	char			*var_name;
	char			*full_path;
	struct s_env	*next;
}	t_env;

typedef struct s_argument_to_expand
{
	char						*content;
	struct s_argument_to_expand	*next;
}		t_argument_to_expand;

typedef struct s_redirection_to_expand
{
	char *arg;
	char						*content;
	enum
	{
		REDIRECTION_OUTFILE,
		REDIRECTION_INFILE,
		REDIRECTION_APPEND,
		// REDIRECTION_HEREDOC
	} type;
	struct s_redirection_to_expand *next;
} t_redirection_to_expand;

typedef struct s_command_to_expand{

	t_argument_to_expand *arguments;
	t_redirection_to_expand *redirections;
	char						*content;
	struct s_command_to_expand *next;
} t_command_to_expand;

typedef struct
{
	int did_succeed;

	t_command_to_expand *commands;

} t_command_line_parsing_result;

typedef struct
{
	int did_succeed;

	t_command_to_expand command;

	char *remaining_line;

} t_command_parsing_result;

typedef struct
{
	int did_succeed;

	t_redirection_to_expand redirection;

	char *remaining_line;

} t_redirection_parsing_result;

typedef struct
{
	int did_succeed;

	t_argument_to_expand argument;

	char *remaining_line;

} t_argument_parsing_result;

/*
* Utilitaries
*/

void	*ft_calloc(size_t nmemb, size_t size);
int		ft_strchr(char *s, int c);
void	ft_putstr_fd(char *s, int fd);
char	*copy(char *s);
char	**ft_split(char const *s, char c);
int		ft_strcmp(char *s1, char *s2);
int		ft_strlen(char const *str);
char	*ft_strjoin(char const *s1, char *s2);
char	*ft_strjoin_arg(char *s1, char *s2);
char	*ft_strjoin_until(char *s1, char *s2, char c);

/*
* Environment
*/

void	init_env(t_env **env, char **envp);

/*
* Signal
*/

void	ft_ctrl_c(int signo);

/*
* Prompt
*/

int		ft_readline(t_env **env, t_vars *vars);

/*
* Parsing
*/

int	ft_find_fist_word(char *str);
char	*skip_spaces(char *str);
char	*skip_one_character(char *str);
char	*skip_quote(
char *str, char c, t_argument_parsing_result *result);
t_command_line_parsing_result ft_parse_command_line(char *command_line);

/*
* Command manager
*/

void	cmd_selector(t_vars *vars, char **env, int ac, char **av);

/*
* Builtins
*/

void	print_env(t_env *envp);

/*
* Chain list
*/

t_command_to_expand	*lst_new_command_parsing_result(void);
t_argument_to_expand	*lst_new_argument_parsing_result(void);
t_env						*ft_lstnew_env(void);
void						ft_lstadd_back_env(t_env **lst, t_env *new);
void						ft_lstclear_env(t_env **lst);
t_env						*lst_search_env(char *s, t_env *env);
t_redirection_to_expand		lst_new_redirection_parsing_result(void);
void	ft_command_to_expand_addback(t_command_to_expand **lst, t_command_to_expand *new);
//t_command_to_expand			ft_command_to_expand_addback(void);
//t_redirection_to_expand		ft_redirection_to_expand_addback(void);
void	ft_redirection_to_expand_addback(t_redirection_to_expand **lst, t_redirection_to_expand *new);
/*
* Free / Error
*/
void	ft_argument_to_expand_addback(t_argument_to_expand **lst, t_argument_to_expand *new);
void	ft_free(char **tab);
void	free_readline(t_vars *vars);
int		check_free_readline(t_vars *vars);

#endif
