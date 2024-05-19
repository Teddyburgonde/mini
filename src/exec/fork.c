/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:03:38 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/19 16:30:05 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	child_process(t_vars *vars, t_redirection *redirect, char **actual_cmd)
{
	if (vars->tmp_fd != -1)
		close(vars->tmp_fd);
	close(vars->pipe_1[0]);
	close(vars->pipe_1[1]);
	close(redirect->infile_fd);
	close(redirect->outfile_fd);
	execve(actual_cmd[0], actual_cmd, vars->env);
	perror("Execve");
	ft_free(vars->path);
	ft_free_tab_3d(vars);
	return (1);
}

int	fork_processes(t_vars *vars, t_command_to_expand *tmp, t_redirection *redirect, t_heredoc *hd)
{
	int	i;

	i = 0;
	ft_heredoc(vars, tmp->redirections);
	while (i < vars->nb_cmd)
	{
		// open_files(vars, tmp->redirections);
		if (pipe(vars->pipe_1) == -1)
			return (EXIT_FAILURE);
		vars->child = fork();
		if (vars->child == 0)
			child_process(vars, redirect, vars->cmd[i]);
		else if (vars->child < 0)
		{
			perror("fork");
			return (EXIT_FAILURE);
		}
		else
		{
			if (vars->tmp_fd != -1)
				close(vars->tmp_fd);
			vars->tmp_fd = dup(vars->pipe_1[0]);
			close(vars->pipe_1[0]);
			close(vars->pipe_1[1]);
		}
		tmp = tmp->next;
		i++;
	}
	return (0);
}