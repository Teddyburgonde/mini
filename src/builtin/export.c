/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:24:05 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/30 15:42:22 by tebandam         ###   ########.fr       */
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

static void	free_full_path(t_env *tmp_env, char *cmd, char *value)
{
	free(tmp_env->full_path);
	tmp_env->full_path = remove_plus(cmd);
	tmp_env->full_path = ft_strjoin_mod(tmp_env->full_path, value);
	tmp_env->value = ft_strjoin_mod(tmp_env->value, value);
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

static void	handle_export_status_2(t_env **env,
		t_env *tmp_env, char *cmd, char *var_name, char *value)
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
