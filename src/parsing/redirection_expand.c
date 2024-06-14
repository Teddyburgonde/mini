/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:33:03 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/14 12:18:27 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_strlen_ultime(t_redirection_to_expand *tmp, t_env *env, t_vars *vars)
{
	int		i;
	int		count;
	int		j;
	char	*var_name;
	char	*exit_code;
	t_env	*node;
	t_bool	in;

	if (!tmp->arg)
		return (0);
	in = FALSE;
	i = 0;
	count = 0;
	while (tmp->arg[i])
	{
		if (tmp->arg[i] == '\'' && in == FALSE)
			in = TRUE;
		else if (tmp->arg[i] == '\'' && in == TRUE)
			in = FALSE;
		else if (tmp->arg[i] == '$' && in == FALSE)
		{
			j = 0;
			var_name = get_var_name(&tmp->arg[i]);
			if (lst_search_env(var_name, env))
			{
				node = lst_search_env(var_name, env);
				if (node->value != NULL && node->value[j] != 0)
				{
					while (node->value[j])
					{
						count++;
						j++;
					}
				}
				count -= ft_strlen(var_name);
			}
			else if (ft_strcmp(var_name, "$?") == 0)
			{
				exit_code = ft_itoa(vars->exit_code);
				count = ft_strlen(exit_code) - 2;
				free(exit_code);
			}
			free(var_name);
		}
		i++;
	}
	count += i;
	return (count);
}

// t_redirection_to_expand	*expand_redirection(t_redirection_to_expand *redirection, t_env *env)
// {
// 	int						i;
// 	int						j;
// 	char					*var_name;
// 	t_redirection_to_expand	*tmp;
// 	t_redirection_to_expand	*final;
// 	t_env					*node;

// 	var_name = NULL;
// 	final = NULL;
// 	while (redirection)
// 	{
// 		i = 0;
// 		j = 0;
// 		tmp = lst_new_redirection_parsing_result();
// 		if (!tmp)
// 			return (NULL);
// 		tmp->arg = malloc(sizeof(char) * ft_strlen_ultime(
// 					redirection, env) + 1);
// 		tmp->e_type = redirection->e_type;
// 		if (ft_strcspn2(redirection->arg, "'") != 0
// 			&& ft_strcspn2(&redirection->arg[i], "$") == 0
// 			&& redirection->arg[i] != '$')
// 		{
// 			tmp->arg[i] = redirection->arg[i];
// 			i++;
// 			while (redirection->arg[i] && redirection->arg[i] != '\'')
// 			{
// 				tmp->arg[i] = redirection->arg[i];
// 				i++;
// 			}
// 			if (redirection->arg[i])
// 			{
// 				tmp->arg[i] = redirection->arg[i];
// 				i++;
// 			}
// 		}
// 		else if (ft_strcspn2(&redirection->arg[i], "$") != 0 ||
// 			redirection->arg[i] == '$')
// 		{
// 			var_name = get_var_name(&redirection->arg[ft_strcspn2(
// 						&redirection->arg[i], "$")]);
// 			if (lst_search_env(var_name, env))
// 			{
// 				node = lst_search_env(var_name, env);
// 				while (env->value[j])
// 				{
// 					tmp->arg[i + j] = node->value[j];
// 					j++;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			while (redirection->arg[i])
// 			{
// 				tmp->arg[i] = redirection->arg[i];
// 				i++;
// 			}
// 		}
// 		tmp->arg[i] = 0;
// 		ft_redirection_to_expand_addback(&final, tmp);
// 		redirection = redirection->next;
// 	}
// 	return (final);
// }

// static t_redirection_to_expand	*ft_get_last_pos(t_redirection_to_expand *lst)
// {
// 	while (lst)
// 	{
// 		if (lst->last_pos == TRUE)
// 			return (lst);
// 		lst = lst->next;
// 	}
// 	return (NULL);
// }

// static int	ft_split_redirection(t_redirection_to_expand *redirection_to_split,
// 	t_redirection_to_expand **redirs, t_redirection_to_expand *origin)
// {
// 	int						in;
// 	t_bool					in_quote;
// 	static int				i = 0;
// 	t_redirection_to_expand	*tmp;
// 	t_redirection_to_expand	*splitted_redirections;

	
// 	splitted_redirections = lst_new_redirection_parsing_result();
// 	if (!splitted_redirections || !redirection_to_split)
// 		return (0);
// 	in = 0;
// 	tmp = redirection_to_split;
// 	if (tmp->arg[0] == '"'
// 		|| tmp->arg[0] == '\'')
// 		in_quote = TRUE;
// 	else
// 		in_quote = FALSE;
// 	while (tmp && tmp->arg[i] && in_quote == FALSE)
// 	{
// 		if (tmp->arg[i] == SPACE
// 			|| tmp->arg[i] == TAB
// 			|| tmp->arg[i] == NEW_LINE)
// 			break ;
// 		else if (in == 0)
// 		{
// 			splitted_redirections->arg = malloc(
// 					ft_strcspn(&tmp->arg[i], " \t\n") + 1);
// 			in++;
// 		}
// 		if (tmp->arg[i])
// 		{
// 			splitted_redirections->arg[i] = tmp->arg[i];
// 			i++;
// 		}
// 	}
// 	while (tmp && tmp->arg[i] && in_quote == TRUE)
// 	{
// 		if (tmp->arg[i] == '\''
// 			|| tmp->arg[i] == '"')
// 		{
// 			if (in == 1)
// 				in_quote = FALSE;
// 			else
// 			{
// 				in++;
// 				if (tmp->arg[i] == '"')
// 				{
// 					splitted_redirections->arg = malloc(
// 							ft_strcspn(&tmp->arg[i + 1], "\""));
// 				}
// 				else
// 				{
// 					splitted_redirections->arg = malloc(
// 							ft_strcspn(&tmp->arg[i + 1], "'"));
// 				}
// 			}
// 		}
// 		if (tmp->arg[i])
// 		{
// 			splitted_redirections->arg[i] = tmp->arg[i];
// 			i++;
// 		}
// 	}
// 	splitted_redirections->arg[i] = 0;
// 	splitted_redirections->e_type = redirection_to_split->e_type;
// 	ft_redirection_to_expand_addback(redirs, splitted_redirections);
// 	if (tmp->arg[i] == 0)
// 	{
// 		origin = origin->next;
// 		redirection_to_split = redirection_to_split->next;
// 		i = 0;
// 	}
// 	else
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd(origin->arg, 2);
// 		ft_putstr_fd(": ambiguous redirect\n", 2);
// 		ft_lstclear_redirections(redirs);
// 		return (0);
// 	}
// 	if (redirection_to_split == NULL)
// 		return (0);
// 	return (1);
// }


// Redirection avec expand peut faire un erreur :
// bash: $file: ambiguous redirect
// Pas de expand sur les heredoc


// t_redirection_to_expand	*ft_expand_redirections(t_redirection_to_expand *redirection,
// 		t_env *env)
// {
// 	t_redirection_to_expand	*expand_redirections;
// 	t_redirection_to_expand	*splitted_redirections;
// 	t_redirection_to_expand	*tmp_split;
// 	int						tmp;
	
// 	//redirection = NULL;
// 	tmp = 1;
// 	expand_redirections = NULL;
// 	splitted_redirections = NULL;
// 	tmp_split = NULL;
// 	expand_redirections = expand_redirection(redirection, env);
// 	tmp_split = expand_redirections;
// 	if (expand_redirections)
// 	{
// 		while (tmp != 0)
// 		{
// 			tmp = ft_split_redirection(tmp_split,
// 					&splitted_redirections, redirection);
// 		}
// 	}
// 	tmp_split = splitted_redirections;
// 	while (tmp_split)
// 	{
// 		if (tmp_split->arg[0] == '\'' || tmp_split->arg[0] == '"')
// 			tmp_split->arg = ft_remove_simple_quote(tmp_split->arg);
// 		tmp_split = tmp_split->next;
// 	}
// 	ft_lstclear_redirections(&expand_redirections);
// 	return (splitted_redirections);
// }

// Rework redirection parsing in progress !

/*
* expand_redirection(redirection, env);
* Si des simples quotes sont trouvées alors traiter tel que
* des caractères ordinaires quoi qu'il en soit.
*
* Faire une fonction qui cherche si un expand doit être fait,
* de cette manière l'étape précédente n'est plus nécessaire.
*
* Expand toutes les redirections.
* Si l'expand existe pas alors on le remplace par '/0' sauf
* si la variable a un nom incorrect !
*/

static char	*ft_remove_simple_quote(char *src)
{
	char	*dest;
	int		i;
	int		j;

	i = 1;
	j = 0;

	dest = malloc(sizeof(char *) + ft_strlen(src) - 1);
	while (src[i])
	{
		dest[j] = src[i];
		i++;
		j++;
		if (src[i + 1] == '\0')
			break ;
	}
	dest[j] = '\0';
	return (dest);
}

static void	refresh_quotes_status(t_bool *in_quote, char current)
{
	if (current != 0)
	{
		if (*in_quote && *in_quote == TRUE && current == '\'')
			*in_quote = FALSE;
		else if (*in_quote && *in_quote == FALSE && current == '\'')
			*in_quote = TRUE;
	}
}

static int	need_to_be_expand(t_redirection_to_expand *redirection,
	t_env *env)
{
	int		i;
	t_bool	in_quote;
	char	*var_name;

	i = 0;
	in_quote = FALSE;
	while (redirection->arg && redirection->arg[i])
	{
		refresh_quotes_status(&in_quote, redirection->arg[i]);
		if (redirection->arg[i] == '$' && in_quote == FALSE)
		{
			var_name = get_var_name(&redirection->arg[i]);
			if (lst_search_env(var_name, env) || ft_strcmp(var_name, "$?") == 0)
			{
				free (var_name);
				return (TRUE);
			}
			else
			{
				free (var_name);
				return (2);
			}
		}
		i++;
	}
	return (FALSE);
}

t_redirection_to_expand	*expand_redirection(
	t_redirection_to_expand *redirect, t_env *env, t_vars *vars)
{
	int						i;
	int						j;
	char					*var_name;
	char					*exit_code;
	t_bool					in_quote;
	t_redirection_to_expand	*final;
	t_redirection_to_expand	*tmp;
	t_env					*var;

	var_name = NULL;
	in_quote = FALSE;
	final = NULL;
	while (redirect)
	{
		in_quote = FALSE;
		i = 0;
		tmp = lst_new_redirection_parsing_result();
		if (!tmp)
			return (NULL);
		// Condition inexacte si plusieurs expand a voir si on laisse X
		if (need_to_be_expand(redirect, env) > 0
			&& redirect->e_type != REDIRECTION_HEREDOC)
		{
			// Faire second malloc si non expand
			tmp->arg = ft_calloc(sizeof(char) * ft_strlen_ultime(
						redirect, env, vars) + 2, 1);
			if (!tmp->arg)
				return (NULL);
		}
		// else if (redirect->e_type == UNASIGNED
		// 	|| redirect->e_type == REDIRECTION_HEREDOC)
		// {
		// 	tmp->arg = ft_calloc(
		// 		sizeof(char) * ft_strlen(redirect->arg) + 1, 1);
		// }
		tmp->e_type = redirect->e_type;
		if (need_to_be_expand(redirect, env) > 0
			&& tmp->e_type != REDIRECTION_HEREDOC)
		{
			while (redirect->arg[i])
			{
				refresh_quotes_status(&in_quote, redirect->arg[i]);
				if (in_quote == FALSE && redirect->arg[i] == '$')
					var_name = get_var_name(&redirect->arg[i]);
				if (ft_strcmp(var_name, "$?") == 0)
				{
					j = 0;
					exit_code = ft_itoa(vars->exit_code);
					while (exit_code[j])
					{
						tmp->arg[ft_strlen(tmp->arg)] = exit_code[j];
						j++;
					}
					i += ft_strlen(var_name) - 1;
					free(var_name);
					free(exit_code);
				}
				else if (in_quote == FALSE && redirect->arg[i] == '$')
				{
					j = 0;
					var = lst_search_env(var_name, env);
					if (var != NULL && var->value != NULL && var->value[j] != 0)
					{
						while (var->value[j])
						{
							tmp->arg[ft_strlen(tmp->arg)] = var->value[j];
							j++;
						}
					}
					i += ft_strlen(var_name) - 1;
					free(var_name);
				}
				else
					tmp->arg[ft_strlen(tmp->arg)] = redirect->arg[i];
				i++;
			}
		}
		else
			tmp->arg = copy_without_quote(redirect->arg);
		ft_redirection_to_expand_addback(&final, tmp);
		redirect = redirect->next;
	}
	return (final);
}

static int	ft_error_ambiguous(t_redirection_to_expand *tmp, t_env *env)
{
	int		i;
	char	*var_name;
	t_bool	in_quote;
	t_env	*var;

	i = 0;
	in_quote = FALSE;
	while (tmp->arg[i])
	{
		refresh_quotes_status(&in_quote, tmp->arg[i]);
		if (in_quote == FALSE && tmp->arg[i] == '$')
			var_name = get_var_name(&tmp->arg[i]);
		if (in_quote == FALSE && tmp->arg[i] == '$')
		{
			if (ft_strcmp(var_name, "$?") != 0)
			{
				var = lst_search_env(var_name, env);
				if (var == NULL || var->value == NULL || ft_strlen(var->value) == 0
					|| ft_strcspn(var->value, " \n\t") != ft_strlen(var->value))
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(var_name, 2);
					ft_putstr_fd(": ambiguous redirect\n", 2);
					free(var_name);
					return (1);
				}
				else
					free(var_name);
			}
			else
				free(var_name);
		}
		i++;
	}
	return (0);
}

t_redirection_to_expand	*ft_expand_redirections(t_redirection_to_expand **redirection,
		t_env *env, t_vars *vars)
{
	t_redirection_to_expand	*to_expand;
	t_redirection_to_expand	*expand_redirections;
	t_redirection_to_expand	*error_ambig;
	t_redirection_to_expand	*tmp;

	expand_redirections = NULL;
	to_expand = *redirection;
	expand_redirections = expand_redirection(to_expand, env, vars);
	if (expand_redirections)
	{
		error_ambig = to_expand;
		tmp = expand_redirections;
		while (tmp)
		{
			if (ft_error_ambiguous(error_ambig, env) == 1)
			{
				free(tmp->arg);
				tmp->arg = NULL;
			}
			else if (tmp->arg[0] == '\'' || tmp->arg[0] == '"')
				tmp->arg = ft_remove_simple_quote(tmp->arg);
			tmp = tmp->next;
			error_ambig = error_ambig->next;
		}
	}
	ft_lstclear_redirections(redirection);
	return (expand_redirections);
}
