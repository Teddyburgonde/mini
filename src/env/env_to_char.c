/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:44:13 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/29 15:43:54 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	**sort_env(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	if (!env || !env[0])
		return (NULL);
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
	return (env);
}

char	**env_to_char(t_env *env)
{
	char	**tmp;
	int		i;

	tmp = malloc((ft_lstsize_env(env) + 1) * sizeof(char *));
	if (!tmp)
		return (NULL);
	i = 0;
	while (env)
	{
		tmp[i] = env->full_path;
		env = env->next;
		i++;
	}
	tmp[i] = 0;
	tmp = sort_env(tmp);
	return (tmp);
}
