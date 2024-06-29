/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 07:00:41 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/29 07:01:06 by tebandam         ###   ########.fr       */
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
