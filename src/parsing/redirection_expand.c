/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:33:03 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/13 11:41:39 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_strlen_ultime(t_redirection_to_expand *tmp, t_env *env)
{
	int		i;
	int		count;
	int		j;
	char	*var_name;
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
				while (node->var[j])
				{
					count++;
					j++;
				}
				count -= ft_strlen(var_name);
			}
		}
		i++;
	}
	count += i;
	return (count);
}

t_redirection_to_expand	*expand_redirection(t_redirection_to_expand *redirection, t_env *env)
{
	int						i;
	int						j;
	char					*var_name;
	t_redirection_to_expand	*tmp;
	t_redirection_to_expand	*final;
	t_env					*node;

	var_name = NULL;
	final = NULL;
	while (redirection)
	{
		i = 0;
		j = 0;
		tmp = lst_new_redirection_parsing_result();
		if (!tmp)
			return (NULL);
		tmp->arg = malloc(sizeof(char) * ft_strlen_ultime(
					redirection, env) + 1);
		tmp->e_type = redirection->e_type;
		if (ft_strcspn(redirection->arg, "'") != 0
			&& ft_strcspn(&redirection->arg[i], "$") == 0
			&& redirection->arg[i] != '$')
		{
			tmp->arg[i] = redirection->arg[i];
			i++;
			while (redirection->arg[i] && redirection->arg[i] != '\'')
			{
				tmp->arg[i] = redirection->arg[i];
				i++;
			}
			if (redirection->arg[i])
			{
				tmp->arg[i] = redirection->arg[i];
				i++;
			}
		}
		else if (ft_strcspn(&redirection->arg[i], "$") != 0 ||
			redirection->arg[i] == '$')
		{
			var_name = get_var_name(&redirection->arg[ft_strcspn(
						&redirection->arg[i], "$")]);
			if (lst_search_env(var_name, env))
			{
				node = lst_search_env(var_name, env);
				while (env->var[j])
				{
					tmp->arg[i + j] = node->var[j];
					j++;
				}
			}
		}
		else
		{
			tmp->arg[i] = node->var[i];
			i++;
		}
		ft_redirection_to_expand_addback(&final, tmp);
		redirection = redirection->next;
	}
	return (final);
}

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

static int	ft_split_redirection(t_redirection_to_expand *redirection_to_split,
	t_redirection_to_expand **redirs, t_redirection_to_expand *origin)
{
	int						in;
	t_bool					in_quote;
	static int				i = 0;
	t_redirection_to_expand	*tmp;
	t_redirection_to_expand	*splitted_redirections;

	splitted_redirections = lst_new_redirection_parsing_result();
	if (!splitted_redirections || !redirection_to_split)
		return (0);
	in = 0;
	tmp = redirection_to_split;
	if (tmp->arg[0] == '"'
		|| tmp->arg[0] == '\'')
		in_quote = TRUE;
	else
		in_quote = FALSE;

	// Comment split ?
	// Essayons la meme


	while (tmp && tmp->arg[i] && in_quote == FALSE)
	{
		if (tmp->arg[i] == SPACE
			|| tmp->arg[i] == TAB
			|| tmp->arg[i] == NEW_LINE)
			break ;
		else if (in == 0)
		{
			splitted_redirections->arg = malloc(
					ft_strcspn(&tmp->arg[i], " \t\n") + 1);
			in++;
		}
		if (tmp->arg[i])
		{
			splitted_redirections->arg[i] = tmp->arg[i];
			i++;
		}
	}
	while (tmp && tmp->arg[i] && in_quote == TRUE)
	{
		if (tmp->arg[i] == '\''
			|| tmp->arg[i] == '"')
		{
			if (in == 1)
				in_quote = FALSE;
			else
			{
				in++;
				if (tmp->arg[i] == '"')
				{
					splitted_redirections->arg = malloc(
							ft_strcspn(&tmp->arg[i + 1], "\""));
				}
				else
				{
					splitted_redirections->arg = malloc(
							ft_strcspn(&tmp->arg[i + 1], "'"));
				}
			}
		}
		if (tmp->arg[i])
		{
			splitted_redirections->arg[i] = tmp->arg[i];
			i++;
		}
	}
	splitted_redirections->arg[i] = 0;
	splitted_redirections->e_type = redirection_to_split->e_type;
	ft_redirection_to_expand_addback(redirs, splitted_redirections);
	if (tmp->arg[i] == 0)
	{
		origin = origin->next;
		redirection_to_split = redirection_to_split->next;
		i = 0;
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(origin->arg, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		ft_lstclear_redirections(redirs);
		return (0);
	}
	if (redirection_to_split == NULL)
		return (0);
	return (1);
}

char	*ft_remove_simple_quote(char *src)
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

// Redirection avec expand peut faire un erreur :
// bash: $file: ambiguous redirect
// Pas de expand sur les heredoc


t_redirection_to_expand	*ft_expand_redirections(t_redirection_to_expand *redirection,
		t_env *env)
{
	t_redirection_to_expand	*expand_redirections;
	t_redirection_to_expand	*splitted_redirections;
	t_redirection_to_expand	*tmp_split;
	int						tmp;

	tmp = 1;
	expand_redirections = NULL;
	splitted_redirections = NULL;
	tmp_split = NULL;
	expand_redirections = expand_redirection(redirection, env);
	tmp_split = expand_redirections;
	if (expand_redirections)
	{
		while (tmp != 0)
		{
			tmp = ft_split_redirection(tmp_split,
					&splitted_redirections, redirection);
		}
	}
	tmp_split = splitted_redirections;
	while (tmp_split)
	{
		if (tmp_split->arg[0] == '\'' || tmp_split->arg[0] == '"')
			tmp_split->arg = ft_remove_simple_quote(tmp_split->arg);
		tmp_split = tmp_split->next;
	}
	return (splitted_redirections);
}
