/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:03:38 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/20 15:56:00 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

int	process(t_vars *vars, t_redirection *redirect, t_env **env)
{
	vars->child = fork();
	if (vars->child == 0)
		child_process(vars, redirect, vars->cmd[vars->cmd_index - 1], env);
	else if (vars->child < 0)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	handle_pipe_closing(vars);
	if (vars->cmd_index == vars->nb_cmd)
		vars->last_child = vars->child;
	return (0);
}

int	fork_processes(t_vars *vars, t_redirection **redirect, t_env **envp)
{
	initialize_vars(vars);
	if (vars->nb_cmd > 0)
		process_commands(vars, redirect, envp);
	wait_process(vars);
	ft_close_fd(vars);
	ft_lstclear_final_redirection(redirect);
	ft_free_tab_3d(vars);
	return (0);
}
