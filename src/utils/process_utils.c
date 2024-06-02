/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 21:22:31 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/01 21:28:08 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int    child_process(t_vars *vars, t_redirection *redirect
        , char **actual_cmd, t_env **envp)
{
    ft_flow_redirection(vars, redirect);
    if (redirect->infile_fd == -1 || redirect->outfile_fd == -1)
    {
        perror("Error opening files");
        exit(1);
    }
    ft_close_fd(vars);
    if (is_builtins_exec(vars) == 1) {
        cmd_selector(envp, vars->cmd[vars->cmd_index - 1]);
        exit(0);
    }
    execve(actual_cmd[0], actual_cmd, vars->env);
    ft_close_fd(vars);
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
    perror("Execve");
    return (1);
}

void	close_pipe(int *pipe_fd)
{
	if (pipe_fd[0] != -1)
	{
		close(pipe_fd[0]);
		pipe_fd[0] = -1;
	}
	if (pipe_fd[1] != -1)
	{
		close(pipe_fd[1]);
		pipe_fd[1] = -1;
	}
}

void	handle_pipe_closing(t_vars *vars)
{
	if ((vars->cmd_index - 1) % 2 == 1 && vars->nb_cmd > 1)
	{
		close_pipe(vars->pipe_1);
		if (vars->pipe_2[0] != -1)
		{
			close(vars->pipe_2[0]);
			vars->pipe_2[0] = -1;
		}
	}
	else if ((vars->cmd_index - 1) % 2 == 0 && vars->nb_cmd > 1)
	{
		close_pipe(vars->pipe_2);
		if (vars->pipe_1[0] != -1)
		{
			close(vars->pipe_1[0]);
			vars->pipe_1[0] = -1;
		}
	}
}