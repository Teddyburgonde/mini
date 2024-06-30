/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:24:05 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/29 15:30:36 by rgobet           ###   ########.fr       */
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
	if (ft_strlen(str) == ft_strcspn(str, "="))
	{
		while (str[i])
		{
			result[i] = str[i];
			i++;
		}
	}
	else
	{
		while (str[i] != '=')
		{
			result[i] = str[i];
			i++;
		}
	}
	result[i] = 0;
	return (result);
}

static char	*make_var(char *str)
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

static int	verif_export(char *str)
{
	int		i;
	int		len_mid;
	t_bool	append;

	i = 0;
	append = TRUE;
	if (str[0] == '_' && str[1] == '\0')
		return (0);
	if (ft_isdigit(str[0]) || str[0] == '=')
	{
		ft_putstr_fd("minishell: export: `", 2);
		write (2, str, ft_strlen(str));
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	len_mid = ft_strcspn(str, "=");
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
				if (append == FALSE && str[i] != '_'
					&& i <= len_mid)
				{
					ft_putstr_fd(
						" not a valid identifier\n", 2);
					return (1);
				}
				else if (append == TRUE && str[i] != '_'
					&& i < len_mid)
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
	if ((len_mid == ft_strlen(str) && append == FALSE)
		|| (len_mid + 1 == ft_strlen(str) && append == TRUE))
		return (2);
	return (0);
}

static void	hide_and_update_env_var(t_env *tmp_env, char *cmd)
{
	if (tmp_env->value)
		free(tmp_env->value);
	tmp_env->hide = TRUE;
	if (tmp_env->full_path)
		free(tmp_env->full_path);
	tmp_env->full_path = copy(cmd);
	tmp_env->value = NULL;
}

static void	update_env_var(t_env *tmp_env, char *cmd, char *value)
{
	free(tmp_env->value);
	if (value == NULL || value[0] == 0)
		tmp_env->hide = TRUE;
	else
		tmp_env->hide = FALSE;
	if (tmp_env->value)
		free(tmp_env->value);
	tmp_env->value = copy(value);
	free(tmp_env->full_path);
	tmp_env->full_path = copy(cmd);
}

static void	free_full_path(t_env *tmp_env, char *cmd, char *value)
{
	free(tmp_env->full_path);
	tmp_env->full_path = remove_plus(cmd);
	tmp_env->full_path = ft_strjoin_mod(tmp_env->full_path, value);
	tmp_env->value = ft_strjoin_mod(tmp_env->value, value);
}

static void	add_modified_env_var(t_env **env, char *cmd,
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

static void	add_new_env_var(t_env **env, char *cmd,
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

static void	handle_export_status_0(t_env **env, t_env *tmp_env,
	char *cmd, char *var_name, char *value)
{
	t_env	*tmp;

	if (!tmp_env && var_name[ft_strlen(var_name) - 1] != '+')
	{
		if (!tmp_env)
			add_new_env_var(env, cmd, var_name, value, FALSE);
		else
			update_env_var(tmp_env, cmd, value);
	}
	else if (var_name[ft_strlen(var_name) - 1] == '+')
	{
		var_name[ft_strlen(var_name) - 1] = 0;
		tmp = lst_search_env(var_name, *env);
		var_name[ft_strlen(var_name)] = '+';
		if (!tmp)
			add_modified_env_var(env, cmd, var_name, value);
		else
			free_full_path(tmp, cmd, value);
	}
	else
		update_env_var(tmp_env, cmd, value);
}

static void	handle_export_status_2(t_env **env, t_env *tmp_env, char *cmd, char *var_name, char *value)
{
	if (!tmp_env)
	{
		add_new_env_var(env, cmd, var_name, value, TRUE);
		tmp_env = lst_search_env(var_name, *env);
		tmp_env->value = NULL;
		tmp_env->hide = TRUE;
	}
	else
		hide_and_update_env_var(tmp_env, cmd);
}

int	export(t_env **env, char **cmd)
{
	t_env	*tmp_env;
	char	*var_name;
	char	*value;
	int		export_status;
	int		i;

	i = 1;
	export_status = 0;
	while (cmd[i] != NULL)
	{
		value = NULL;
		var_name = NULL;
		export_status = verif_export(cmd[i]);
		if (export_status == 1)
		{
			i++;
			continue ;
		}
		var_name = make_var_name(cmd[i]);
		tmp_env = lst_search_env(var_name, *env);
		if (export_status == 0)
		{
			value = make_var(cmd[i]);
			handle_export_status_0(env, tmp_env, cmd[i], var_name, value);
		}
		else if (export_status == 2)
		{
			handle_export_status_2(env, tmp_env, cmd[i], var_name, value);
			export_status = 0;
		}
		if (var_name)
			free(var_name);
		if (value)
			free(value);
		i++;
	}
	return (export_status);
}
