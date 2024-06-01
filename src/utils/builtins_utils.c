/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 11:47:35 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/01 12:08:32 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_bool	is_builtins_exec(t_vars *vars)
{
	if (ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "unset") == 0
		|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "export") == 0
		|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "cd") == 0
		|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "pwd") == 0
		|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "echo") == 0
		|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "exit") == 0
		|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "env") == 0)
		return (TRUE);
	return (FALSE);
}

t_bool	is_builtins_parsing(char **str)
{
	if (ft_strcmp(str[0], "unset") == 0
		|| ft_strcmp(str[0], "export") == 0
		|| ft_strcmp(str[0], "cd") == 0
		|| ft_strcmp(str[0], "pwd") == 0
		|| ft_strcmp(str[0], "echo") == 0
		|| ft_strcmp(str[0], "exit") == 0
		|| ft_strcmp(str[0], "env") == 0)
		return (TRUE);
	return (FALSE);
}