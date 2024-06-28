/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_cmd_selector.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 12:43:39 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/28 21:43:24 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	manage_echo_pwd(char **command_line, t_redirection *redirect, t_vars **vars)
{
	if (ft_strcmp(command_line[0], "echo") == 0
		&& ft_strlen(command_line[0]) > 0)
	{
		(*vars)->exit_code = ft_echo(command_line, *vars, redirect);
		return (0);
	}
	else if (ft_strcmp(command_line[0], "pwd") == 0
		&& ft_strlen(command_line[0]) > 0)
	{
		(*vars)->exit_code = ft_pwd(*vars, redirect);
		return (0);
	}
	return (1);
}

int	manage_cd_unset(char **command_line, t_vars **vars, t_env **env)
{
	if (ft_strcmp(command_line[0], "cd") == 0 && ft_strlen(command_line[0]) > 0)
	{
		(*vars)->exit_code = ft_cd(command_line, env);
		return (0);
	}
	else if (ft_strcmp(command_line[0], "unset") == 0
		&& ft_strlen(command_line[0]) > 0)
	{
		(*vars)->exit_code = unset(env, command_line);
		return (0);
	}
	return (1);
}

int	manage_export(char **command_line,
	t_redirection *redirect, t_vars **vars, t_env **env)
{
	char	**envp;
	t_env	*tmp;

	tmp = *env;
	envp = NULL;
	if (ft_strcmp(command_line[0], "export") == 0
		&& ft_strlen(command_line[0]) > 0)
	{
		if (command_line[1])
			(*vars)->exit_code = export(env, command_line);
		else
		{
			envp = env_to_char_export(tmp);
			print_env(envp, *vars, redirect);
			if (envp)
				ft_free(envp);
		}
		return (0);
	}
	return (1);
}

int	manage_exit_env(char **command_line, t_vars **vars, t_redirection *redirect)
{
	if (ft_strcmp(command_line[0], "exit") == 0
		&& ft_strlen(command_line[0]) > 0)
	{
		(*vars)->exit_code = ft_exit(command_line);
		return (0);
	}
	else if ((ft_strcmp(command_line[0], "printenv") == 0
			|| ft_strcmp(command_line[0], "env") == 0)
		&& ft_strlen(command_line[0]) > 0)
	{
		exec_env(command_line, *vars, redirect);
		return (0);
	}
	return (1);
}

int	cmd_selector(t_env **env, char **command_line,
		t_vars *vars, t_redirection *redirect)
{
	if (redirect->ambiguous == TRUE)
	{
		vars->exit_code = 1;
		return (0);
	}
	if (vars->nb_cmd > 1 && vars->child != 0)
		return (1);
	if (manage_echo_pwd(command_line, redirect, &vars) == 0)
		return (0);
	if (manage_cd_unset(command_line, &vars, env) == 0)
		return (0);
	if (manage_export(command_line, redirect, &vars, env) == 0)
		return (0);
	if (manage_exit_env(command_line, &vars, redirect) == 0)
		return (0);
	return (1);
}
