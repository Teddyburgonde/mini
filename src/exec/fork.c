/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:03:38 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/15 14:04:12 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	child_process(t_vars *vars, char *envp[], int actual_cmd)
{
	ft_flow_redirection(vars, actual_cmd);
	if (vars->tmp_fd != -1)
		close(vars->tmp_fd);
	close(vars->pipe_1[0]);
	close(vars->pipe_1[1]);
	close(vars->fd_infile);
	close(vars->fd_outfile);
	execve(vars->cmd[actual_cmd][0], vars->cmd[actual_cmd], envp);
	perror("Execve");
	ft_free(vars->path);
	ft_free_tab_3d(vars);
	return (1);
}

int	fork_processes(t_vars *vars, char *envp[], t_command_to_expand *tmp)
{
	int	i;

	i = 0;
	ft_heredoc(vars, tmp->redirections);
	while (i < vars->nb_cmd)
	{
		open_files(vars, tmp->redirections);
		if (pipe(vars->pipe_1) == -1)
			return (EXIT_FAILURE);
		vars->child = fork();
		if (vars->child == 0)
			child_process(vars, envp, i);
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