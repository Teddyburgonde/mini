/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redirection_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 18:41:04 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/29 20:35:55 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_existing_var(t_redirection_to_expand *redirection,
	t_env *env, int i)
{
	char	*var_name;

	var_name = get_var_name(&redirection->arg[i]);
	if (lst_search_env(var_name, env) || ft_strcmp(var_name, "$?") == 0
		|| (var_name[0] == '$' && var_name[1] == '?'))
	{
		free (var_name);
		return (TRUE);
	}
	else
	{
		free (var_name);
		return (2);
	}
	return (0);
}

static int	need_to_be_expand(t_redirection_to_expand *redirection,
	t_env *env)
{
	int		i;
	t_bool	in_quote;

	i = 0;
	in_quote = FALSE;
	while (redirection->arg && redirection->arg[i])
	{
		refresh_quotes_status(&in_quote, redirection->arg[i]);
		if (redirection->arg[i] == '$' && in_quote == FALSE)
		{
			if (is_existing_var(redirection, env, i) == TRUE)
				return (TRUE);
			else if (is_existing_var(redirection, env, i) == 2)
				return (2);
		}
		i++;
	}
	return (FALSE);
}

static void	setup_expand_redirection(t_redirection_to_expand **tmp,
	t_redirection_to_expand *redirect, t_env *env, t_vars *vars)
{
	*tmp = lst_new_redirection_parsing_result();
	if (!tmp)
		return ;
	if (need_to_be_expand(redirect, env) > 0
		&& redirect->e_type != REDIRECTION_HEREDOC)
	{
		(*tmp)->arg = ft_calloc(sizeof(char) * ft_strlen_ultime(
					redirect, env, vars) + 2, 1);
		if (!(*tmp)->arg)
			return ;
	}
}

t_redirection_to_expand	*get_redirection_list(
	t_redirection_to_expand *redirect, t_env *env, t_vars *vars)
{
	int						i;
	t_redirection_to_expand	*final;
	t_redirection_to_expand	*tmp;

	final = NULL;
	while (redirect)
	{
		i = 0;
		setup_expand_redirection(&tmp, redirect, env, vars);
		tmp->e_type = redirect->e_type;
		if (need_to_be_expand(redirect, env) > 0
			&& tmp->e_type != REDIRECTION_HEREDOC)
			i = expand_redirection(&tmp, vars, env, redirect);
		else
			tmp->arg = copy_without_quote(redirect->arg);
		ft_redirection_to_expand_addback(&final, tmp);
		redirect = redirect->next;
	}
	return (final);
}
