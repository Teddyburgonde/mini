/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 11:05:59 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/21 10:30:00 by rgobet           ###   ########.fr       */
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


typedef enum bool {
	TRUE = 1,
	FALSE = 0
}	t_bool;

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



// arg1 < re'dir2' arg2 arg3 < redir1

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

/*
* Environment
*/

void	init_env(t_env **env, char **envp);

/*
* Signal
*/

void	ft_signal(void);
void	ft_ctrl_c(int signo);

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
/*
* Command manager
*/

void	cmd_selector(char *command_line, char **env, int ac, char **av);

/*
* Builtins
*/

void	print_env(t_env *envp);

/*
* Chain list
*/

t_env	*ft_lstnew_env(void);
t_env	*lst_search_env(char *s, t_env *env);

t_argument_to_expand				*lst_new_argument_parsing_result(void);
t_command_to_expand					*lst_new_command_parsing_result(void);
t_redirection_to_expand				*lst_new_redirection_parsing_result(void);

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

/*
* Free / Error
*/

void	ft_free(char **tab);
void	ft_free_vars_input(char *command_line, char **env);



// TODO

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

// typedef struct s_char_list t_char_list;
// struct s_char_list
// {
	
// 	char	value;
// 	t_bool	was_in_a_variable;

// 	t_char_list *next;
// };

// typedef struct s_argument t_argument;
// struct s_argument
// {
// 	t_char_list *chars;

// 	t_argument* next;	
// };


// t_argument *ft_expand_vars_in_argument(const char* argument)
// {
// 	// pour chaque charactere de l'argument:
// 		// si je suis une quote simple ':
// 			// je vais chercher la quote simple suivante. (ELLE EXISTE NECESSAIREMENT)
// 			// et je remplace tous les characteres rencontrés sans les marquer.
// 		// sinon
// 			// si je suis un dollar $:
// 				// si je suis suivi de \0:
// 					// j'ecris un dollar
// 				// sinon:
// 					// je remplace en marquant les characteres (was_in_a_variable = TRUE)
// 			// sinon je remplace le charactere sans le marquer.
// }

// t_argument *ft_split_argument(const t_argument *argument_to_split)
// {
// 	t_argument *splitted_arguments;

// 	splitted_arguments = NULL;

// 	// iterer sur les character de argument_to_split

// 	// couper aux espaces.

// 	return (splitted_arguments);
// }

// t_argument* ft_expand_argument(const t_argument_to_expand argument)
// {
// 	t_argument *argument_with_expanded_vars;
// 	t_argument *splitted_arguments;
// 	t_argument *final_arguments;
	
// 	// deux étapes:
// 	//  1) remplacement des variables + flaguer les characteres -> t_char_list *
// 	argument_with_expanded_vars = ft_expand_vars_in_argument(argument.content);
// 	// 2) split aux espaces qui sont pas entre quotes.
// 	splitted_arguments = ft_split_argument(argument_with_expanded_vars);
// 	// 3) virer les quotes qui sont pas flaguees
// 	final_arguments = ft_remove_quotes_from_arguments(splitted_arguments);

// 	return (final_arguments);
	
// }


#endif