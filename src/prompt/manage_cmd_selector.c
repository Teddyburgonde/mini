/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_cmd_selector.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 12:43:39 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/29 07:24:16 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
	if (manage_env(command_line, &vars, redirect) == 0)
		return (0);
	if (vars->nb_cmd == 1 && manage_exit(command_line, &vars) == 0)
		return (0);
	return (1);
}
