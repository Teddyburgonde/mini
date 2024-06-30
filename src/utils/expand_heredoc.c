/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 15:16:27 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/29 20:35:25 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	exit_code(t_vars *vars, char **str, int *j, int i)
{
	char	*exit_code;

	*j = 0;
	exit_code = ft_itoa(vars->exit_code);
	while (exit_code && exit_code[*j])
	{
		str[0][i + *j] = exit_code[*j];
		*j += 1;
	}
	free(exit_code);
}

static int	expand(char *var_name, t_env *env, int i, char **str)
{
	int		j;
	t_env	*var;

	j = 0;
	var = lst_search_env(var_name, env);
	if (var)
	{
		while (var->value && var->value[j])
		{
			str[0][i + j] = var->value[j];
			j++;
		}
	}
	return (j);
}

static void	last_part(char *tmp, char *var_name, int *t, int *j)
{
	if (tmp[*t] == '$' && tmp[*t + 1] == '?')
		*t += 1;
	else if (var_name == NULL || var_name[0] == 0)
		*t = skip_dolar_var(tmp, *t);
	else
		*t = skip_dolar_var(tmp, *t) - 1;
	free(var_name);
	*j -= 1;
}

static void	while_expand(char *tmp, char **str, t_env *env, t_vars *vars)
{
	int		i;
	int		t;
	int		j;
	char	*var_name;

	init_expand(&i, &j, &t, &var_name);
	while (tmp && tmp[t])
	{
		i += j;
		if (tmp[t] == '$')
		{
			var_name = get_var_name(&tmp[t]);
			if (tmp[t] == '$' && (tmp[t + 1] == ' '
					|| tmp[t + 1] == '\t' || tmp[t + 1] == '\n'))
				*str[i] = tmp[t];
			else if (var_name[0] == '$' && var_name[1] == '?')
				exit_code(vars, str, &j, i);
			else if (var_name != NULL || var_name[0] != 0)
				j = expand(var_name, env, i, str);
			last_part(tmp, var_name, &t, &j);
		}
		else
			str[0][i] = tmp[t];
		increment_index(&i, &t, tmp);
	}
}

char	*expand_line(char *str, t_env *env, t_vars *vars)
{
	int		size;
	char	*tmp;

	size = ft_strlen_with_expand(str, env, vars) + 1;
	tmp = copy(str);
	free(str);
	str = malloc(sizeof(char) * size);
	if (!str)
		return (str);
	while_expand(tmp, &str, env, vars);
	str[ft_strlen_with_expand(str, env, vars)] = 0;
	free(tmp);
	return (str);
}
