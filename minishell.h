/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 11:05:59 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/31 13:03:55 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <limits.h>
# define NEW_LINE '\n'
# define SPACE ' '
# define RIGHT '>'
# define LEFT '<'
# define TAB '\t'
# define PIPE '|'


# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef enum s_bool {
	TRUE = 1,
	FALSE = 0
}	t_bool;

/*
* Global variables
*/
// Va surement degager (e_last)
typedef struct s_vars {
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
}	t_vars;

/*
* Environment
*/

typedef struct s_env {
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

typedef struct s_command_to_expand {

	t_argument_to_expand		*arguments;
	t_redirection_to_expand		*redirections;

	struct s_command_to_expand	*next;
}	t_command_to_expand;

typedef struct command_line_parsing_result {

	t_bool				did_succeed;

	t_command_to_expand	*commands;

}	t_command_line_parsing_result;

typedef struct command_parsing_result {
	t_bool				did_succeed;

	t_command_to_expand	*command;

	const char			*remaining_line;

}	t_command_parsing_result;

typedef struct redirection_parsing_result {
	t_bool					did_succeed;

	t_redirection_to_expand	*redirection;

	const char				*remaining_line;

}	t_redirection_parsing_result;

typedef struct argument_parsing_result {

	t_bool					did_succeed;

	t_argument_to_expand	*argument;

	const char				*remaining_line;

}	t_argument_parsing_result;

/*
* Expand parsing 
*/

typedef struct s_char_list {

	char				value;
	t_bool				last_pos;
	t_bool				was_in_a_variable;

	struct s_char_list	*next;
}	t_char_list;

typedef struct s_argument {

	t_char_list			*chars;

	struct s_argument	*next;	
}	t_argument;

/*
* Expand redirections
*/

// e_type va peut-etre sauter

typedef struct s_redirection {
  int  infile_fd;
  int  outfile_fd;
  char	*limiter;
  enum
  {
	HERE,
	COMING,
	NONE
  }	e_position;
  enum
  {
	PIPE_OUT,
	STDOUT 
  } e_type;
  struct s_redirection  *next;
}  t_redirection;

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
int		single_redirection(char *str);
int		double_redirection(char *str);
int		ft_strcspn(const char *s, char *reject);
int		skip_dolar_var(char *argument, int index);
int		ft_atoi(const char *str);
char	*ft_substr_gnl(char const *s, unsigned int start, size_t len);
char	*get_next_line(int fd);
int		ft_lstsize_command(t_command_to_expand *cmd);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
int		ft_lstsize_env(t_env *env);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcspn2(const char *s, char *reject);
void	ft_exit_message_0(void);
void	ft_exit_message_2(char *command);
void	ft_exit_message_too_many_arguments(void);
void	ft_exit_message_argument_required(char *command);
int		ft_isdigit(int c);

/*
* Env
*/

void	init_env(t_env **env, char **envp);
char	**env_to_char(t_env *env);
char	**env_to_char_export(t_env *env);

/*
* Signal
*/

void	ft_ctrl_c(int signal);
void	ctrl_d(int signal);

/*
* Prompt
*/

int		ft_readline(t_env **env);

/*
* Parsing
*/

char	*skip_spaces(const char *str);
char	*skip_one_character(const char *str);
char	*skip_quote(
			const char *str, char c, t_argument_parsing_result *result);
char	*ft_skip_arg(const char *str, char *reject);
char	*ft_strjoin_until(char *s1, const char *s2, char *reject);
char	*ft_strjoin_arg(char *s1, const char *s2);
char	*ft_strjoin_file(char *s1, const char *s2);
char	*ft_strjoin_quoted_arg(char *s1, const char *s2, char *reject);

t_command_line_parsing_result	*ft_parse_command_line(char *command_line);

t_redirection_parsing_result	*parse_redirection(char *str);

t_command_parsing_result		*ft_allocated_result(void);
t_command_parsing_result		*redirections(
			t_command_parsing_result *result, char *remaining_line,
			t_redirection_parsing_result *redirection_result);
t_command_parsing_result		*arguments(
			t_command_parsing_result *result,
			t_argument_parsing_result *argument_result, char *remaining_line);

t_argument_parsing_result		*parse_quote(const char *remaining_line,
			t_argument_parsing_result *result);
t_argument_parsing_result		*is_parsing_arg(const char *remaining_line,
			t_argument_parsing_result *result);

t_command_parsing_result		*parse_command(char *command_line);
t_command_parsing_result		*ft_redirections_arguments(
			char **remaining_line, t_command_parsing_result *result,
			t_redirection_parsing_result *redirection_result,
			t_argument_parsing_result *argument_result);
t_argument						*ft_expand_argument(
			const t_argument_to_expand *argument, t_env *env);

/*
* Setup command
*/

char	**ft_setup_command(t_argument *arg);

/*
* Command manager
*/

int		ft_cmd_manager(t_env **env, t_command_line_parsing_result *cmd);
int		cmd_selector(t_env **env, char **command_line);

/*
* Builtins
*/

void	export(t_env **env, char **cmd);
int		ft_cd(char **command, t_env **env);
void	ft_echo(char **command);
int		ft_pwd(void);
void	unset(t_env **env, char *name);
int		ft_exit(char **command);

/*
* Chain list
*/

t_env	*ft_lstnew_env(void);
t_env	*lst_search_env(char *s, t_env *env);

t_argument_to_expand				*lst_new_argument_parsing_result(void);
t_command_to_expand					*lst_new_command_parsing_result(void);
t_redirection_to_expand				*lst_new_redirection_parsing_result(void);
t_char_list							*lst_new_char_list(void);
t_argument							*lst_new_argument(void);

void	ft_lstclear_env(t_env **lst);
void	ft_lstclear_commands(t_command_to_expand **lst);
void	ft_lstclear_arguments(t_argument_to_expand **lst);
void	ft_lstclear_redirections(t_redirection_to_expand **lst);
void	ft_lstadd_back_env(t_env **lst, t_env *new);
void	ft_command_to_expand_addback(
			t_command_to_expand **lst, t_command_to_expand *new);
void	ft_redirection_to_expand_addback(
			t_redirection_to_expand **lst, t_redirection_to_expand *new);
void	ft_argument_to_expand_addback(
			t_argument_to_expand **lst, t_argument_to_expand *new);
void	ft_lstadd_back_argument(t_argument **lst, t_argument *new);
void	ft_lstadd_back_char_list(t_char_list **lst, t_char_list *new);
void	ft_lstadd_back_redirection(t_redirection **lst, t_redirection *new);


t_redirection	*ft_lstnew_redirection(void);

// int		ft_lstsize_expand(t_char_list *lst);

void	ft_lstclear_argument(t_argument **lst);
void	ft_lstclear_char_list(t_char_list **lst);

/*
* Free / Error
*/

void	ft_free(char **tab);
void	ft_free_vars_input(char *command_line, char **env);
void	ft_free_tab_3d(t_vars *vars);
void 	ft_lstclear_final_redirection(t_redirection **lst);


// ici 

// # ifndef BUFFER_SIZE
// #  define BUFFER_SIZE 1
// # endif

// typedef struct s_vars{
// 	enum{
// 		APPEND,
// 		OUTFILE,
// 		UNASIGN
// 	}	e_last;
// 	pid_t	child;
// 	int		fd_infile;
// 	int		fd_outfile;
// 	int		fd_append;
// 	int		nb_out;
// 	int		nb_cmd;
// 	char	**path;
// 	char	***cmd;
// 	char	**full_cmd;
// 	int		pipe_1[2];
// 	int		tmp_fd;
// 	// A malloc sur nb_hd, la liste chainee sera parcouru lors de l'utilisation des hd
// 	int		nb_hd;
// 	int		hd_w;
// 	int		hd_r;
// }	t_vars;

/*
* Exec
*/

int	ft_cmd_manager(t_env **env, t_command_line_parsing_result *cmd);
t_redirection	*stock_redirection(t_command_to_expand *list);
t_redirection_to_expand	*is_last(t_command_to_expand *cmd);
void	ft_heredoc(t_redirection *redirection, t_bool save);
int	check_infile(t_redirection_to_expand *redir);
int	open_files(t_vars *vars, t_redirection_to_expand *redir);
char	**find_the_accessible_path(char **path, t_vars *vars, char **command_line);
void	build_path(char **path, char **bin_path, char **is_valid_cmd, char **full_cmd);
void	update_full_cmd(char ***full_cmd, char *is_valid_cmd);
int		verif_fill_command_paths(t_vars *vars, t_command_to_expand *tmp, t_env *env);
int		fork_processes(t_vars *vars, t_redirection **redirect, t_env **envp);
void	capture_and_redirection(char *tab, char *tmp, t_vars *vars);
void	open_fd_infile(t_vars *vars);
void	open_hd_w(t_vars *vars);
void	verif_tab(char *tab);


t_bool	is_builtins_exec(t_vars *vars);
t_bool	is_builtins_parsing(char **str);


/*
* GNL
*/

char	*ft_strjoin_mod(char *s1, char *s2);
char	*read_loop(char *buf, char *stock, int *len, int fd);
char	*ft_strchr_rl(const char *s, int c);
char	*read_line(int fd, char *stock);
char	*ft_strdup_gnl(const char *s);
char	*ft_substr_gnl(char const *s, unsigned int start, size_t len);
char	*extract_line(char *stock);
char	*ft_strdup(const char *s);
char	*extract_surplus_line(char *stock);
char	*get_next_line(int fd);









t_redirection	*ft_lstlast_redirection(t_redirection *lst);














// No needed now

// t_argument_to_expand *original_arguments;
// t_argument_to_expand *arguments_with_parameters_expanded =NULL;
// t_argument_to_expand *arguments_with_quotes_expanded =NULL;

// while ()


// a appeller pour chaque argument (ou argument de redirection)
// t_argument_to_expand* ft_expand_parameters(t_argument_to_expand argument) {

// }

// t_argument_to_expand* ft_expand_quotes(t_argument_to_expand argument) {

// }



// parsing 
// fork , ecouter exit status du dernier
// waitpid 

// cas particulier : une seul command et que c'est un builtind -> il ne faut pas fork !
// builtind dans le process parent si il y a une seul command
// sur le cd et le pwd est set avec env
// si la taille est de 1 , faire une autre route 

// parsing dans les fork 
// couper les espaces -> problematique -> si dans la variable tu as une
// quote

// flag caractere , associer une valeur pour dire si elle est special ou pas 
// struct le caractere et le bool 
// 

// "" '' $ 


// printf "%s\n" $VAR

// yi
// yfieif
// ihwf

// printf "%s\n" "$VAR"

// yi yfieif ihwf



// Si $VAR existe pas echo va print du vide mais qu'est-ce que 
// sa fait pour d'autre fonction


/*
* Split sur SPACE / NEW_LINE / TAB et si le premier char 
* est une double quote ou simple
* t_arg contient les arg et on avance a chaque appelle de fonction 
* envoie l'adresse de la liste chainee final qui contient 
* les arg avec les variable expand split.
*
* Le was_in_variable est important pour eviter d'expand une
* variable dans une autre variable par exemple.
*/



// rgobet@2G5:~$ printf "%s\n" "$VAR"
// test test
// rgobet@2G5:~$ printf "%s\n" $VAR
// test
// test

// chars -> liste chainee
// 	value -> char
// 	was_in_variable -> bool

// Si il existe redef la var en question sinon ajoute a la liste
// export vide affiche env
// unset sans rien sa fait rien
// env si tu met un truc derriere il le prend comme un fichier si il existe il se passe rien
// sinon il dit que le fichier n'existe pas si il y a un fichier existant et d'autre argument
// env fait rien

t_redirection_to_expand	*ft_expand_redirections(t_redirection_to_expand *redirection,
		t_env *env);
char	*get_var_name(char *str);

#endif