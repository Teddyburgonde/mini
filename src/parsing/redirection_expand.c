/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:33:03 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/29 20:22:07 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	error_message_expand_redir(char *var_name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(var_name, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	free(var_name);
}

static int	exit_code_redir(char *var_name, t_env *env)
{
	t_env	*var;

	var = NULL;
	if (var_name[0] != '$' && var_name[1] != '?')
	{
		var = lst_search_env(var_name, env);
		if (var == NULL || var->value == NULL
			|| ft_strlen(var->value) == 0
			|| ft_strcspn(var->value, " \n\t") != ft_strlen(var->value))
		{
			error_message_expand_redir(var_name);
			return (1);
		}
		else
			free(var_name);
	}
	else
		free(var_name);
	return (0);
}

static int	ft_error_ambiguous(t_redirection_to_expand *tmp, t_env *env)
{
	int		i;
	char	*var_name;
	t_bool	in_quote;

	i = 0;
	in_quote = FALSE;
	while (tmp->arg[i])
	{
		refresh_quotes_status(&in_quote, tmp->arg[i]);
		if (in_quote == FALSE && tmp->arg[i] == '$')
			var_name = get_var_name(&tmp->arg[i]);
		if (in_quote == FALSE && tmp->arg[i] == '$')
		{
			if (exit_code_redir(var_name, env) == 1)
				return (1);
		}
		i++;
	}
	return (0);
}

static int	ambiguous_redirection(t_redirection_to_expand *error_ambig,
	t_env *env, t_redirection_to_expand	*tmp)
{
	if (ft_error_ambiguous(error_ambig, env) == 1)
	{
		free(tmp->arg);
		tmp->arg = NULL;
		return (1);
	}
	return (0);
}

t_redirection_to_expand	*ft_expand_redirections(
	t_redirection_to_expand **redirection,
		t_env *env, t_vars *vars)
{
	t_redirection_to_expand	*to_expand;
	t_redirection_to_expand	*expand_redirections;
	t_redirection_to_expand	*error_ambig;
	t_redirection_to_expand	*tmp;

	expand_redirections = NULL;
	to_expand = *redirection;
	expand_redirections = get_redirection_list(to_expand, env, vars);
	if (expand_redirections)
	{
		error_ambig = to_expand;
		tmp = expand_redirections;
		while (tmp)
		{
			if (ambiguous_redirection(error_ambig, env, tmp) == 0
				&& (tmp->arg[0] == '\'' || tmp->arg[0] == '"'))
				tmp->arg = ft_remove_simple_quote(tmp->arg);
			tmp = tmp->next;
			error_ambig = error_ambig->next;
		}
	}
	ft_lstclear_redirections(redirection);
	return (expand_redirections);
}
