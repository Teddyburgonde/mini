/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_processes_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 20:20:15 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/02 13:45:52 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	wait_process(t_vars *vars)
{
	int		wstatus;
	int		exit_status;
	pid_t	pid;

	wstatus = 0;
	pid = 0;
	while (pid != -1)
	{
		pid = wait(&wstatus);
		if (pid == vars->last_child)
			exit_status = WEXITSTATUS(wstatus);
	}
	return (exit_status);
}

void	initialize_vars(t_vars *vars)
{
	vars->cmd_index = 1;
    vars->pipe_1[0] = -1;
    vars->pipe_1[1] = -1;
    vars->pipe_2[0] = -1;
    vars->pipe_2[1] = -1;
    vars->last_child = -2;
}

int	setup_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	return (0);
}

int	choice_pipe_setup(t_vars *vars)
{
	if ((vars->cmd_index - 1) % 2 == 1)
		return (setup_pipe(vars->pipe_1));
	else
		return (setup_pipe(vars->pipe_2));
}

void	process_commands(t_vars *vars, t_redirection **redirect, t_env **envp)
{
	t_redirection *tmp;

	tmp = *redirect;
	while (vars->cmd_index <= vars->nb_cmd)
	{
		if (choice_pipe_setup(vars) == 1)
			return ;
		if (vars->nb_cmd >= 2
			|| tmp->infile_fd > 2
			|| tmp->outfile_fd > 2
			|| is_builtins_exec(vars) == 0)
			process(vars, tmp, envp);
		tmp = tmp->next;
		vars->cmd_index++;
	}
}
