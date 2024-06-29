/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:33:03 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/26 15:11:00 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	strlen_expand(char *var_name, t_env *env, int *count, int *j)
{
	t_env	*node;

	node = lst_search_env(var_name, env);
	if (node->value != NULL && node->value[*j] != 0)
	{
		while (node->value[*j])
		{
			*count += 1;
			*j += 1;
		}
	}
	*count -= ft_strlen(var_name);
}

static void	strlen_exit_code(t_vars *vars, int *count)
{
	char	*exit_code;

	exit_code = ft_itoa(vars->exit_code);
	*count = ft_strlen(exit_code) - 2;
	free(exit_code);
}

static int expand_len(t_redirection_to_expand *tmp,
	t_vars *vars, t_env *env, int i)
{
	int		j;
	int		count;
	char	*var_name;

	j = 0;
	count = 0;
	var_name = get_var_name(&tmp->arg[i]);
	if (lst_search_env(var_name, env))
		strlen_expand(var_name, env, &count, &j);
	else if (var_name[0] == '$' && var_name[1] == '?')
		strlen_exit_code(vars, &count);
	free(var_name);
	return (count);
}

static int	ft_strlen_ultime(t_redirection_to_expand *tmp,
	t_env *env, t_vars *vars)
{
	int		i;
	int		count;
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
			count += expand_len(tmp, vars, env, i);
		i++;
	}
	count += i;
	return (count);
}

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

static void	function_one(t_vars *vars, t_redirection_to_expand	*tmp,
	char *var_name, int *i)
{
	int		j;
	char	*exit_code;

	j = 0;
	exit_code = ft_itoa(vars->exit_code);
	while (exit_code[j])
	{
		tmp->arg[ft_strlen(tmp->arg)] = exit_code[j];
		j++;
	}
	*i += ft_strlen(var_name) - 1;
	free(var_name);
	free(exit_code);
}

static void	function_two(char *var_name, t_env *env,
	t_redirection_to_expand	*tmp, int *i)
{
	int		j;
	t_env	*var;

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
	*i += ft_strlen(var_name) - 1;
	free(var_name);
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

static t_bool	is_in_quote(char *str, int i)
{
	int		j;
	t_bool	in_quote;

	j = 0;
	in_quote = FALSE;
	while (str && str[j])
	{
		if (str[j] == '\'' && in_quote == FALSE)
			in_quote = TRUE;
		else if (str[j] == '\'' && in_quote == TRUE)
			in_quote = FALSE;
		if (j == i)
			return (in_quote);
		j++;
	}
	return (in_quote);
}

static int	function_three(t_redirection_to_expand **tmp,
	t_vars *vars, t_env *env, t_redirection_to_expand *redirect)
{
	static int	i = 0;
	char		*var_name;
	t_bool		in_quote;

	var_name = NULL;
	in_quote = FALSE;
	i = ft_strcspn(&redirect->arg[i], "$");
	while (redirect->arg[i])
	{
		in_quote = is_in_quote(redirect->arg, i);
		if (in_quote == FALSE && redirect->arg[i] == '$')
			var_name = get_var_name(&redirect->arg[i]);
		if (ft_strcmp(var_name, "$?") == 0)
			function_one(vars, *tmp, var_name, &i);
		else if (in_quote == FALSE && redirect->arg[i] == '$')
			function_two(var_name, env, *tmp, &i);
		else
			(*tmp)->arg[ft_strlen((*tmp)->arg)] = redirect->arg[i];
		i++;
	}
	return (i);
}

t_redirection_to_expand	*expand_redirection(
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
			i = function_three(&tmp, vars, env, redirect);
		else
			tmp->arg = copy_without_quote(redirect->arg);
		ft_redirection_to_expand_addback(&final, tmp);
		redirect = redirect->next;
	}
	return (final);
}

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
