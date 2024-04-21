/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 11:05:59 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/21 15:09:25 by rgobet           ###   ########.fr       */
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
# define NEW_LINE '\n'
# define SPACE ' '
# define RIGHT '>'
# define LEFT '<'
# define TAB '\t'
# define PIPE '|'


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




// typedef struct s_char_list {

// 	char		value;
// 	t_bool		was_in_a_variable;

// 	s_char_list	*next;
// }	t_char_list;

// typedef struct s_argument {
// 	t_char_list	*chars;

// 	s_argument	*next;	
// }	t_argument;

// t_char_list	*lst_new_chars_list(void)
// {
// 	t_char_list	*tmp;

// 	tmp = malloc(sizeof(t_char_list));
// 	if (!tmp)
// 		return (NULL);
// 	tmp->value = NULL;
// 	tmp->was_in_a_variable = FALSE;
// 	tmp->next = NULL;
// 	return (tmp);
// }

// t_argument	*lst_new_argument(void)
// {
// 	t_argument	*tmp;

// 	tmp = malloc(sizeof(t_argument));
// 	if (!tmp)
// 		return (NULL);
// 	tmp->chars = NULL;
// 	tmp->next = NULL;
// 	return (tmp);
// }

// void	ft_lstadd_back_char_list(t_char_list **lst, t_char_list *new)
// {
// 	t_char_list	*tmp;

// 	if (!*lst)
// 	{
// 		*lst = new;
// 		return ;
// 	}
// 	tmp = *lst;
// 	while (tmp && tmp->next != NULL)
// 		tmp = tmp->next;
// 	tmp->next = new;
// }

// char	*get_var_name(char *str)
// {
// 	char	*var_name;
// 	int		i;

// 	i = 0;
// 	var_name = malloc((strcspn(str, "<>\'\"| \n\t") + 1) * sizeof(char));
// 	if (!var_name)
// 		return (NULL);
// 	while (str[i] != SPACE && str[i] != NEW_LINE && str[i] != TAB &&
// 		str[i] != PIPE && str[i] != LEFT && str[i] != RIGHT &&
// 		str[i] != '\'' && str[i] != '"')
// 	{
// 		var_name[i] = str[i];
// 		i++;
// 	}
// 	var_name[i] = 0;
// 	return (var_name);
// }

// int	skip_dolar_var(char *argument, int index)
// {
// 	int	i;

// 	i = index;
// 	while (argument[i] != SPACE && argument[i] != NEW_LINE &&
// 		argument[i] != TAB && argument[i] != PIPE &&
// 		argument[i] != LEFT && argument[i] != RIGHT &&
// 		argument[i] != '\'' && argument[i] != '"')
// 		i++;
// 	return (i);
// }

// t_argument *ft_expand_vars_in_argument(const char* argument, t_env *env)
// {
// 	t_char_list	*tmp;
// 	t_argument	*arg;
// 	t_env		*var;
// 	int			i;
// 	int			j;

// 	i = 0;
// 	arg = lst_new_argument();
// 	if (!arg)
// 		return (NULL);
// 	while (argument[i])
// 	{
// 		j = 0;
// 		if (argument[i] == '\'')
// 		{
// 			tmp = lst_new_chars_list();
// 			if (!tmp)
// 				return (NULL);
// 			tmp->value = argument[i];
// 			ft_lstadd_back_char_list(&arg->chars, tmp);
// 			i++;
// 			while (argument[i] != '\'')
// 			{
// 				tmp = lst_new_chars_list();
// 				if (!tmp)
// 					return (NULL);
// 				tmp->value = argument[i];
// 				ft_lstadd_back_char_list(&arg->chars, tmp);
// 				i++;
// 			}
// 			tmp = lst_new_chars_list();
// 			if (!tmp)
// 				return (NULL);
// 			tmp->value = argument[i];
// 			ft_lstadd_back_char_list(&arg->chars, tmp);
// 		}
// 		else
// 		{
// 			if (argument[i] == '$'
// 				&& lst_search_env(get_var_name(&argument[i]), env))
// 			{
// 				var = lst_search_env(get_var_name(&argument[i]), env);
// 				while (var->var[j])
// 				{
// 					tmp = lst_new_chars_list();
// 					if (!tmp)
// 						return (NULL);
// 					tmp->value = env->var[j];
// 					tmp->was_in_a_variable = TRUE;
// 					ft_lstadd_back_char_list(&arg->chars, tmp);
// 				}
// 			}
// 			else
// 				i = skip_dolar_var(argument, i);
// 		}
// 		i++;
// 	}
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


typedef struct s_splitted_argument {

	char				*argument;

	s_splitted_argument	*next;	
}	t_splitted_argument;

t_splitted_argument	lst_new_splitted_argument(void)
{
	t_splitted_argument	*tmp;

	tmp = malloc(sizeof(t_splitted_argument));
	if (!tmp)
		return (NULL);
	tmp->argument = NULL;
	tmp->next = NULL;
	return (tmp);
}

void	ft_lstadd_back_splitted_argument(t_splitted_argument **lst,
	t_splitted_argument *new)
{
	t_splitted_argument	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp && tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
}

/*
* Split sur SPACE / NEW_LINE / TAB et si le 
*/

t_splitted_argument	*ft_split_argument(const t_argument *argument_to_split)
{
	t_splitted_argument	*splitted_arguments;

	splitted_argument = lst_new_splitted_argument();
	if (!splitted_arguments)
		return (NULL);
	if (old == NULL)
		old->next = splitted_arguments;
	if (argument_to_split->argument != '\'' && argument_to_split->argument)
	splitted_arguments->argument = malloc(
			(strcspn(argument_to_split, " \t\n") + 1) * sizeof(char));
	while (argument_to_split->argument[i] != SPACE
		&& argument_to_split->argument[i] != NEW_LINE
		&& argument_to_split->argument[i] != TAB
		&& argument_to_split->argument[i] != '\''
		&& argument_to_split->argument[i] != '"')
	{
		splitted_arguments->argument[i] = argument_to_split[i];
		i++;
	}
	splitted_arguments->argument[i] = 0;
	if (argument_to_split[i] != 0 && old == NULL)
		ft_split_argument(argument_to_split, splitted_arguments, i);
	// iterer sur les character de argument_to_split
	// couper aux espaces.
	return (splitted_arguments);
}

// char	*ft_remove_quotes(char *src)
// {
// 	char	*dest;
// 	int		i;
// 	int		j;

// 	i = 1;
// 	j = 0;

// 	dest = malloc(sizeof(char *) + ft_strlen(src) - 1);
// 	while (src[i])
// 	{
// 		dest[j] = src[i];
// 		i++;
// 		j++;
// 		if (src[i + 1] == '\0')
// 			break ;
// 	}
// 	dest[j] = '\0';
// 	return (dest);
// }

// t_argument* ft_expand_argument(const t_argument_to_expand argument)
// {
// 	t_argument *argument_with_expanded_vars;
// 	t_argument *splitted_arguments;
// 	t_argument *final_arguments;

// 	// deux étapes:
// 	//  1) remplacement des variables + flaguer les characteres -> t_char_list * dans un while
// 	argument_with_expanded_vars = ft_expand_vars_in_argument(argument.content);
// 	// 2) split aux espaces qui sont pas entre quotes.
// 	splitted_arguments = ft_split_argument(argument_with_expanded_vars);
// 	// 3) virer les quotes qui sont pas flaguees
// 	final_arguments = ft_remove_quotes_from_arguments(splitted_arguments);

// 	return (final_arguments);

// }


#endif