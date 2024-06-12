/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 21:22:31 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/12 16:47:18 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	child_process(t_vars *vars, t_redirection *redirect
		, char **actual_cmd)
{
	if (redirect->ambiguous == TRUE)
		exit(1);
	ft_flow_redirection(vars, redirect);
	if (redirect->infile_fd == -1 || redirect->outfile_fd == -1)
	{
		perror("Error opening files");
		exit(1);
	}
	ft_close_fd(vars);
	if (actual_cmd != NULL && is_builtins_exec(vars) == 1)
		exit(0);
	if (actual_cmd == NULL)
		exit (0);
	execve(actual_cmd[0], actual_cmd, vars->env);
	ft_close_fd(vars);
	error_close_files(redirect);
	perror("Execve");
	return (1);
}

void	error_close_files(t_redirection *redirect)
{
	if (redirect->infile_fd != -1)
	{
		close(redirect->infile_fd);
		redirect->infile_fd = -1;
	}
	if (redirect->outfile_fd != -1)
	{
		close(redirect->outfile_fd);
		redirect->outfile_fd = -1;
	}
}

// void	close_pipe(int *pipe_fd)
// {
// 	if (pipe_fd[0] != -1)
// 	{
// 		close(pipe_fd[0]);
// 		pipe_fd[0] = -1;
// 	}
// 	if (pipe_fd[1] != -1)
// 	{
// 		close(pipe_fd[1]);
// 		pipe_fd[1] = -1;
// 	}
// }

void	handle_pipe_closing(t_vars *vars)
{
	if ((vars->cmd_index - 1) % 2 == 1 && vars->nb_cmd > 1)
	{
		// close_pipe(vars->pipe_1);
		if (vars->pipe_1[1] != -1)
		{
			close(vars->pipe_1[1]);
			vars->pipe_1[1] = -1;
		}
		if (vars->pipe_2[0] != -1)
		{
			close(vars->pipe_2[0]);
			vars->pipe_2[0] = -1;
		}
	}
	else if ((vars->cmd_index - 1) % 2 == 0 && vars->nb_cmd > 1)
	{
		// close_pipe(vars->pipe_2);
		if (vars->pipe_1[0] != -1)
		{
			close(vars->pipe_1[0]);
			vars->pipe_1[0] = -1;
		}
		if (vars->pipe_2[1] != -1)
		{
			close(vars->pipe_2[1]);
			vars->pipe_2[1] = -1;
		}
	}
}
