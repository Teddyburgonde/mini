/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:44:13 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/03 12:07:53 by rgobet           ###   ########.fr       */
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

static char	*setup_full_path(char *str)
{
	int		i;
	int		j;
	int		len;
	char	*result;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	len = ft_strlen(str) + 3;
	if (ft_strchr(str, '=') == 0)
		len -= 2;
	result = malloc(len * sizeof(char *));
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (j == i && i > 0 && str[i - 1] == '=')
			result[j] = '"';
		else
		{
			result[j] = str[i];
			i++;
		}
		j++;
	}
	if (str[i - 1] == '=')
	{
		result[j] = '"';
		j++;
	}
	if (ft_strchr(str, '=') != 0)
		result[j] = '"';
	if (ft_strchr(str, '=') != 0)
		j++;
	result[j] = 0;
	return (result);
}

char	**env_to_char_export(t_env *env)
{
	char	**tmp;
	int		i;

	tmp = malloc((ft_lstsize_env(env) + 1) * sizeof(char *));
	if (!tmp)
		return (NULL);
	i = 0;
	while (env)
	{
		tmp[i] = setup_full_path(env->full_path);
		env = env->next;
		i++;
	}
	tmp[i] = 0;
	tmp = sort_env(tmp);
	return (tmp);
}

static int	count_hidden_var(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		if (env->hide == TRUE)
			i++;
		env = env->next;
	}
	return (i);
}

char	**env_to_char(t_env *env)
{
	char	**tmp;
	int		len;
	int		i;

	len = ft_lstsize_env(env);
	len -= count_hidden_var(env);
	tmp = malloc((len + 1) * sizeof(char *));
	if (!tmp)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->hide == FALSE)
			tmp[i] = env->full_path;
		else
			i--;
		env = env->next;
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}
