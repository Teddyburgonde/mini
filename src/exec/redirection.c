/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:54:32 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/15 14:46:15 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_flow_redirection(t_vars *vars, int actual_cmd)
{
	if (actual_cmd == 0)
	{
		if (dup2(vars->fd_infile, STDIN_FILENO) < 0)
			perror("dup2");
		if (dup2(vars->pipe_1[1], STDOUT_FILENO) < 0)
			perror("dup2");
	}
	else if (actual_cmd == vars->nb_cmd - 1)
	{
		if (vars->e_last == OUTFILE)
		{	
			if (dup2(vars->tmp_fd, STDIN_FILENO) < 0)
				perror("dup2");
			if (dup2(vars->fd_outfile, STDOUT_FILENO) < 0)
				perror("dup2");
		}
		else if (vars->e_last == APPEND)
		{
			if (dup2(vars->tmp_fd, STDIN_FILENO) < 0)
				perror("dup2");
			if (dup2(vars->fd_append, STDOUT_FILENO) < 0)
				perror("dup2");
		}
	}
	else
	{
		// ???
		if (dup2(vars->tmp_fd, STDIN_FILENO) < 0)
			perror("dup2");
		if (dup2(vars->pipe_1[1], STDOUT_FILENO) < 0)
			perror("dup2");
	}
}

void	capture_and_redirection(char *tab, char *tmp, t_vars *vars)
{
	while (ft_strncmp(tab, tmp, ft_strlen(tmp)))
	{
		ft_putstr_fd(tab, vars->hd_w);
		free(tab);
		write(1, "> ", 2);
		tab = get_next_line(STDIN_FILENO);
		if (!tab)
		{
			ft_putstr_fd("Error\n No limiter for heredoc", 2);
			exit(1);
		}
	}
	free(tab);
	free(tmp);
	close(vars->hd_w);
}

