/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_expand_utils_two.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 18:30:36 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/29 20:39:22 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	redirection_expand_exit_code(t_vars *vars,
		t_redirection_to_expand	*tmp,
	char *var_name, int *i)
{
	int		j;
	char	*exit_code;

	printf("function one\n");
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

static void	redirection_expand_var(char *var_name, t_env *env,
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

int	expand_redirection(t_redirection_to_expand **tmp,
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
			redirection_expand_exit_code(vars, *tmp, var_name, &i);
		else if (in_quote == FALSE && redirect->arg[i] == '$')
			redirection_expand_var(var_name, env, *tmp, &i);
		else
			(*tmp)->arg[ft_strlen((*tmp)->arg)] = redirect->arg[i];
		i++;
	}
	return (i);
}
