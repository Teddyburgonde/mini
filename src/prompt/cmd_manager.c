/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 15:10:36 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/24 11:35:39 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_cmd_manager(t_env **env,
	t_command_line_parsing *cmd, t_vars *vars)
{
	t_command_to_expand				*tmp;
	t_argument_to_expand			*tmp_arg;
	t_redirection					*redirection;

	tmp = cmd->commands;
	tmp_arg = tmp->arguments;
	if (*env)
	{
		manage_variable_path(env, vars);
		tmp->redirections = ft_expand_redirections(&cmd->commands->redirections,
				*env, vars);
		stock_redirection(cmd->commands, *env, vars, &redirection);
		vars->nb_cmd = ft_lstsize_command(cmd->commands);
		vars->cmd = ft_calloc(vars->nb_cmd + 1, sizeof(char **));
		verif_fill_command_paths(vars, tmp, *env);
		cleanup_vars_path(vars);
		vars->env = env_to_char(*env);
		fork_processes(vars, &redirection, env);
		ft_lstclear_commands(&(cmd->commands));
		free(vars->env);
	}
	return (0);
}
