/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 20:48:43 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/29 21:23:06 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*make_var(char *str)
{
	char	*result;
	int		i;

	i = 0;
	result = NULL;
	if (ft_strchr(str, '=') == 0)
		result = malloc(sizeof(char) * 1);
	else if (ft_strchr(str, '=') > 0 && ft_strchr(str, '+') == 0)
		result = malloc(sizeof(char)
				* (ft_strlen(&str[ft_strcspn(str, "=") + 1]) + 1));
	else if (ft_strchr(str, '=') > 0 && ft_strchr(str, '+') > 0)
		result = malloc(sizeof(char)
				* (ft_strlen(&str[ft_strcspn(str, "=") + 1]) + 2));
	if (!result || !str)
		return (NULL);
	if (ft_strchr(str, '=') > 0)
	{
		while (str[ft_strcspn(str, "=") + i + 1])
		{
			result[i] = str[ft_strcspn(str, "=") + i + 1];
			i++;
		}
	}
	result[i] = 0;
	return (result);
}

char	*remove_plus(char *str)
{
	int		len;
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	len = ft_strcspn(str, "+");
	len += ft_strlen(&str[len + 2]);
	result = malloc(sizeof(char) * (len + 2));
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i + 1] && str[i] == '+' && str[i + 1] == '=')
			i++;
		result[j] = str[i];
		i++;
		j++;
	}
	result[j] = 0;
	return (result);
}

void	add_modified_env_var(t_env **env, char *cmd,
	char *var_name, char *value)
{
	t_env	*tmp_env;
	char	*tmp;

	tmp_env = lst_search_env(var_name, *env);
	if (!tmp_env)
		tmp_env = ft_lstnew_env();
	if (value == NULL || value[0] == 0)
		tmp_env->hide = TRUE;
	else
		tmp_env->hide = FALSE;
	tmp_env->full_path = remove_plus(cmd);
	tmp = ft_substr(
			var_name, 0, ft_strlen(var_name) - 1);
	tmp_env->var_name = tmp;
	tmp_env->value = copy(value);
	tmp_env->next = NULL;
	ft_lstadd_back_env(env, tmp_env);
}

void	add_new_env_var(t_env **env, char *cmd,
	char *var_name, char *value, int hide)
{
	t_env	*tmp_env;

	tmp_env = ft_lstnew_env();
	if (value == NULL || value[0] == 0)
		tmp_env->hide = TRUE;
	else
		tmp_env->hide = hide;
	tmp_env->full_path = copy(cmd);
	tmp_env->var_name = copy(var_name);
	tmp_env->value = copy(value);
	tmp_env->next = NULL;
	ft_lstadd_back_env(env, tmp_env);
}
