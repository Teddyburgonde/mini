/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 18:39:25 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/29 11:18:22 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**sort_env(char **env)
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

void	add_quote_after_equal(char *str, char *result)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (i > 0 && str[i - 1] == '=')
		{
			result[j] = '"';
			j++;
		}
		result[j] = str[i];
		i++;
		j++;
	}
	if (i > 0 && str[i - 1] == '=')
	{
		result[j] = '"';
		j++;
	}
	result[j] = '\0';
}

char	*setup_full_path(char *str)
{
	int		i;
	int		len;
	char	*result;

	i = 0;
	if (!str)
		return (NULL);
	len = ft_strlen(str) + 3;
	if (ft_strchr(str, '=') == 0)
		len -= 2;
	result = malloc(len * sizeof(char *));
	if (!result)
		return (NULL);
	add_quote_after_equal(str, result);
	if (ft_strchr(str, '=') != 0)
	{
		result[len - 2] = '"';
		result[len - 1] = '\0';
	}
	else
		result[len - 1] = '\0';
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

int	count_hidden_var(t_env *env)
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
