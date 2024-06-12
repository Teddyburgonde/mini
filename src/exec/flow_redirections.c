/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flow_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 11:31:26 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/12 15:13:17 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	first_command_redirection(t_vars *vars, t_redirection *redirect)
{
	if (redirect->infile_fd > 2)
	{
		if (dup2(redirect->infile_fd, STDIN_FILENO) < 0)
			perror("dup2");
	}
	if (redirect->outfile_fd > 2)
	{
		if (dup2(redirect->outfile_fd, STDOUT_FILENO) < 0)
			perror("dup2");
	}
	else if (vars->nb_cmd > 1)
	{
		if (dup2(vars->pipe_2[1], STDOUT_FILENO) < 0)
			perror("dup2");
	}
}

void	pipe_command_redirection_even(t_redirection *redirect, t_vars *vars)
{
	if (redirect->infile_fd > 2)
	{
		if (dup2(redirect->infile_fd, STDIN_FILENO) < 0)
			perror("dup2");
	}
	else
	{
		if (dup2(vars->pipe_2[0], STDIN_FILENO) < 0)
			perror("dup2");
	}
	if (redirect->outfile_fd > 2)
	{
		if (dup2(redirect->outfile_fd, STDOUT_FILENO) < 0)
			perror("dup2");
	}
	else
	{
		if (dup2(vars->pipe_1[1], STDOUT_FILENO) < 0)
			perror("dup2");
	}
}

void	pipe_command_redirection_odd(t_redirection *redirect, t_vars *vars)
{
	if (redirect->infile_fd > 2)
	{
		if (dup2(redirect->infile_fd, STDIN_FILENO) < 0)
			perror("dup2");
	}
	else
	{
		if (dup2(vars->pipe_1[0], STDIN_FILENO) < 0)
			perror("dup2");
	}
	if (redirect->outfile_fd > 2)
	{
		if (dup2(redirect->outfile_fd, STDOUT_FILENO) < 0)
			perror("dup2");
	}
	else
	{
		if (dup2(vars->pipe_2[1], STDOUT_FILENO) < 0)
			perror("dup2");
	}
}

void	last_command_redirection_even(t_redirection *redirect, t_vars *vars)
{
	if (redirect->infile_fd > 2)
	{
		if (dup2(redirect->infile_fd, STDIN_FILENO) < 0)
			perror("dup2");
	}
	else if (vars->pipe_2[0] != -1)
	{
		if (dup2(vars->pipe_2[0], STDIN_FILENO) < 0)
			perror("dup2");
	}
	if (redirect->outfile_fd > 2)
	{
		if (dup2(redirect->outfile_fd, STDOUT_FILENO) < 0)
			perror("dup2");
	}
}

void	last_command_redirection_odd(t_redirection *redirect, t_vars *vars)
{
	if (redirect->infile_fd > 2)
	{
		if (dup2(redirect->infile_fd, STDIN_FILENO) < 0)
			perror("dup2");
	}
	else if (vars->pipe_1[0] != -1)
	{
		if (dup2(vars->pipe_1[0], STDIN_FILENO) < 0)
			perror("dup2");
	}
	if (redirect->outfile_fd > 2)
	{
		if (dup2(redirect->outfile_fd, STDOUT_FILENO) < 0)
			perror("dup2");
	}
}
