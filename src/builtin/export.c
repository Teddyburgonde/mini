/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:24:05 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/28 16:05:31 by rgobet           ###   ########.fr       */
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
	while (str[ft_strcspn(str, "=") + i + 1])
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

static int	verif_export(char *str)
{
	int		i;
	int		len_mid;
	t_bool	append;

	i = 0;
	append = TRUE;
	if (str[0] == 0 || str[0] == '_')
		return (1);
	len_mid = ft_strcspn(str, "=");
	if (len_mid == ft_strlen(str))
		return (2);
	if (str[len_mid - 1] == '+')
		len_mid--;
	else
		append = FALSE;
	while (str[i])
	{
		if (str[i] < 48
			|| (str[i] > 57 && str[i] < 65)
			|| (str[i] > 90 && str[i] < 97)
			|| str[i] > 122)
		{
			if ((i != len_mid && append == FALSE)
				|| (i != len_mid + 1 && append == TRUE)
				|| (i != len_mid && append == TRUE))
			{
				if (str[i] != '_')
				{
					ft_putstr_fd(
						"minishell: syntax error near unexpected token `", 2);
					write(2, &str[i], 1);
					write(2, "'\n", 2);
					return (1);
				}
			}
		}
		i++;
	}
	return (0);
}

void	export(t_env **env, char **cmd)
{
	t_env	*tmp_env;
	t_env	*tmp;
	char	*var_name;
	char	*value;
	int		i;

	i = 1;
	if (!cmd[1])
	{
		tmp = *env;
		while (tmp)
		{
			printf("declare -x %s\n", tmp->full_path);
			tmp = tmp->next;
			// Affiche en ordre décroissant
		}
	}
	while (cmd[i] != NULL)
	{
		if (verif_export(cmd[i]) == 1)
			break ;
		if (verif_export(cmd[i]) == 0)
		{
			var_name = make_var_name(cmd[i]);
			value = make_var(cmd[i]);
			tmp_env = lst_search_env(var_name, *env);
			if (!tmp_env)
			{
				tmp = ft_lstnew_env();
				tmp->full_path = copy(cmd[i]);
				tmp->var_name = var_name;
				tmp->value = value;
				tmp->next = NULL;
				if (tmp->value != 0)
					ft_lstadd_back_env(env, tmp);
				else
				{
					free(tmp->var_name);
					free(tmp->value);
					free(tmp);
				}
			}
			else if (var_name[ft_strlen(var_name)] == '+')
			{
				if (!tmp_env)
				{
					tmp = ft_lstnew_env();
					tmp->full_path = remove_plus(cmd[i]);
					tmp->var_name = ft_substr(
							var_name, 0, ft_strlen(var_name) - 1);
					tmp->value = value;
					tmp->next = NULL;
					if (tmp->value != 0)
						ft_lstadd_back_env(env, tmp);
					else
					{
						free(tmp->var_name);
						free(tmp->value);
						free(tmp);
					}
				}
				else
				{
					free(tmp->full_path);
					tmp->full_path = remove_plus(cmd[i]);
					tmp_env->value = ft_strjoin_mod(tmp_env->value, value);
					free(value);
				}
			}
			else
			{
				tmp_env = *env;
				while (!ft_strcmp(tmp_env->var_name, var_name))
					tmp_env = tmp_env->next;
				free(tmp_env->value);
				tmp_env->value = value;
				tmp->full_path = copy(cmd[i]);
			}
		}
		i++;
	}
	// Tri ordre décroissant pour env
}
