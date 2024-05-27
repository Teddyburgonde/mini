/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:24:05 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/27 13:16:00 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*make_var_name(char *str)
{
	char	*result;
	int		i;

	i = 0;
	result = malloc(sizeof(char) * (ft_strcspn(str, "=") + 1));
	if (!result || !str)
		return (NULL);
	while (str[i] != '=')
	{
		result[i] = str[i];
		i++;
	}
	result[i] = 0;
	return (result);
}

static char	*make_var(char *str)
{
	char	*result;
	int		i;

	i = 0;
	result = malloc(sizeof(char)
			* (ft_strlen(&str[ft_strcspn(str, "=") + 1]) + 2));
	if (!result || !str)
		return (NULL);
	while (str[i] != '=')
	{
		result[i] = str[ft_strcspn(str, "=") + i + 1];
		i++;
	}
	result[i] = 0;
	return (result);
}

void	export(t_env **env, char **cmd)
{
	t_env	*tmp_env;
	t_env	*tmp;
	char	*var_name;

	var_name = make_var_name(cmd[1]);
	tmp_env = lst_search_env(var_name, *env);
	if (!cmd[1])
	{
		tmp = *env;
		while (tmp)
		{
			printf("declare -x %s\n", tmp->full_path);
			tmp = tmp->next;
		}
	}
	else if (!tmp_env)
	{
		tmp = ft_lstnew_env();
		tmp->var_name = var_name;
		tmp->var = make_var(cmd[1]);
		tmp->next = NULL;
		ft_lstadd_back_env(env, tmp);
	}
	else
	{
		tmp_env = *env;
		while (!ft_strcmp(tmp_env->var_name, var_name))
			tmp_env = tmp_env->next;
		free(tmp_env->var);
		tmp_env->var = make_var(cmd[1]);
	}
	// Tri ordre décroissant
}
