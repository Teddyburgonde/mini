/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:24:05 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/28 14:05:29 by rgobet           ###   ########.fr       */
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

// Add utils

size_t	ft_min(const char *s, size_t len)
{
	if (len > (size_t)ft_strlen(s))
		return ((size_t)ft_strlen(s));
	return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*tab;

	i = start;
	if (!s)
		return (0);
	if (start >= (unsigned int)ft_strlen(s))
	{
		tab = ft_calloc(1, sizeof(char));
		if (tab == 0)
			return (0);
	}
	else
	{
		len = ft_min(&s[start], len);
		tab = ft_calloc((1 + len), sizeof(char));
		if (tab == 0)
			return (0);
		while (s[i] && len > (i - start))
		{
			tab[i - start] = s[i];
			i++;
		}
	}
	return (tab);
}

static char	*remove_plus(char *str)
{
	int		len;
	int		i;
	char	*result;

	i = 0;
	if (!str)
		return (NULL);
	len = ft_strcspn(str, "+");
	len += ft_strlen(&str[len + 2]);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	while (result[i])
	{
		if (result[i] != '+' && result[i] != '=')
			result[i] = str[i];
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
	char	*value;
	int		i;

	i = 0;
	if (!cmd[1])
	{
		tmp = *env;
		while (tmp)
		{
			printf("declare -x %s\n", tmp->full_path);
			tmp = tmp->next;
		}
	}
	while (cmd[i + 1] != NULL)
	{
		var_name = make_var_name(cmd[1]);
		value = make_var(cmd[1]);
		tmp_env = lst_search_env(var_name, *env);
		if (!tmp_env)
		{
			tmp = ft_lstnew_env();
			tmp->full_path = copy(cmd[i + 1]);
			tmp->var_name = var_name;
			tmp->var = value;
			tmp->next = NULL;
			if (tmp->var != 0)
				ft_lstadd_back_env(env, tmp);
			else
			{
				free(tmp->var_name);
				free(tmp->var);
				free(tmp);
			}
		}
		else if (var_name[ft_strlen(var_name)] == '+')
		{
			if (!tmp_env)
			{
				tmp = ft_lstnew_env();
				tmp->full_path = remove_plus(cmd[i + 1]);
				tmp->var_name = ft_substr(var_name, 0, ft_strlen(var_name) - 1);
				tmp->var = value;
				tmp->next = NULL;
				if (tmp->var != 0)
					ft_lstadd_back_env(env, tmp);
				else
				{
					free(tmp->var_name);
					free(tmp->var);
					free(tmp);
				}
			}
			else
			{
				free(tmp->full_path);
				tmp->full_path = remove_plus(cmd[i + 1]);
				tmp_env->var = ft_strjoin_mod(tmp_env->var, value);
				free(value);
			}
		}
		else
		{
			tmp_env = *env;
			while (!ft_strcmp(tmp_env->var_name, var_name))
				tmp_env = tmp_env->next;
			free(tmp_env->var);
			tmp_env->var = value;
			tmp->full_path = copy(cmd[i + 1]);
		}
		i++;
	}
	// Tri ordre décroissant
}
