/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_processes_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 20:20:15 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/20 12:53:22 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	wait_process(t_vars *vars)
{
	int		wstatus;
	int		exit_status;
	pid_t	pid;

	exit_status = 0;
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
	vars->last_child = 0;
	vars->child = 0;
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
	t_redirection	*tmp;

	tmp = *redirect;
	while (vars->cmd_index <= vars->nb_cmd)
	{
		if (choice_pipe_setup(vars) == 1)
			return ;
		if (vars->nb_cmd >= 2
			|| cmd_selector(envp, vars->cmd[vars->cmd_index - 1],
				vars, *redirect) == 1)
			process(vars, tmp, envp);
		if (vars->cmd[vars->cmd_index - 1] != NULL
			&& (ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "export") == 0
			|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "unset") == 0))
		{
			if (vars->env)
				free(vars->env);
			vars->env = env_to_char(*envp);
		}
		vars->cmd_index++;
		tmp = tmp->next;
	}
}
