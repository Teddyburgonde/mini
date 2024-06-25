/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strlen_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 15:21:14 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/25 15:28:50 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	init_strlen_vars(int *i, int *j, int *count)
{
	*i = 0;
	*j = 0;
	*count = 0;
}

static void	var_inexistant(int *i, int *j, int *count, char *str)
{
	*j = *i;
	*i = skip_dolar_var(str, *i);
	*count -= *j - *i;
}

static int	count_expand(char *var_name, t_env *env)
{
	int		i;
	t_env	*var;

	i = 0;
	var = NULL;
	if (var_name)
	{
		var = lst_search_env(var_name, env);
		if (var)
		{
			while (var->value && var->value[i])
				i++;
		}
	}
	return (i);
}

static int	exit_code_size(t_vars *vars, int *i)
{
	char	*exit_code;
	int		j;

	j = 0;
	exit_code = ft_itoa(vars->exit_code);
	while (exit_code && exit_code[j])
		j++;
	*i += 2;
	free(exit_code);
	return (j);
}

int	ft_strlen_with_expand(char *str, t_env *env, t_vars *vars)
{
	int		i;
	int		j;
	int		count;
	char	*var_name;

	init_strlen_vars(&i, &j, &count);
	while (str && str[i])
	{
		j = 0;
		if (str[i] == '$')
		{
			var_name = get_var_name(&str[i]);
			j += count_expand(var_name, env);
			if (var_name == NULL || var_name[0] == 0)
				var_inexistant(&i, &j, &count, str);
			else if (var_name[0] == '$' && var_name[1] == '?')
				j += exit_code_size(vars, &i);
			else
				i = skip_dolar_var(str, i);
			free(var_name);
		}
		refresh_index(str, &i, &count, j);
	}
	return (count);
}
