/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:54:32 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/09 11:06:05 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <unistd.h>

// void	ft_flow_redirection(t_vars *vars, int actual_cmd)
// {
// 	if (actual_cmd == 0)
// 	{
// 		if (dup2(vars->fd_infile, STDIN_FILENO) < 0)
// 			perror("dup2");
// 		if (dup2(vars->pipe_1[1], STDOUT_FILENO) < 0)
// 			perror("dup2");
// 	}
// 	else if (actual_cmd == vars->nb_cmd - 1)
// 	{
// 		if (vars->e_last == OUTFILE)
// 		{	
// 			if (dup2(vars->tmp_fd, STDIN_FILENO) < 0)
// 				perror("dup2");
// 			if (dup2(vars->fd_outfile, STDOUT_FILENO) < 0)
// 				perror("dup2");
// 		}
// 		else if (vars->e_last == APPEND)
// 		{
// 			if (dup2(vars->tmp_fd, STDIN_FILENO) < 0)
// 				perror("dup2");
// 			if (dup2(vars->fd_append, STDOUT_FILENO) < 0)
// 				perror("dup2");
// 		}
// 	}
// 	else
// 	{
// 		// ???
// 		if (dup2(vars->tmp_fd, STDIN_FILENO) < 0)
// 			perror("dup2");
// 		if (dup2(vars->pipe_1[1], STDOUT_FILENO) < 0)
// 			perror("dup2");
// 	}
// }

// void	capture_and_redirection(char *tab, char *tmp, t_vars *vars)
// {
// 	while (ft_strncmp(tab, tmp, ft_strlen(tmp)))
// 	{
// 		ft_putstr_fd(tab, vars->hd_w);
// 		free(tab);
// 		write(1, "> ", 2);
// 		tab = get_next_line(STDIN_FILENO);
// 		if (!tab)
// 		{
// 			ft_putstr_fd("Error\n No limiter for heredoc", 2);
// 			exit(1);
// 		}
// 	}
// 	free(tab);
// 	free(tmp);
// 	close(vars->hd_w);
// }


// Trouve le dernier heredoc de TOUTES LES CMDS
// Return le dernier node qui est de e_type == REDIRECTION_HEREDOC

t_redirection_to_expand	*is_last(t_redirection_to_expand *tmp)
{
	t_redirection_to_expand	*result;

	result = NULL;
	while (tmp)
	{
		if (tmp->e_type == REDIRECTION_HEREDOC)
			result = tmp;
		tmp = tmp->next;
	}
	return (result);
}

void	ft_flow_redirection(t_vars *vars, t_redirection *redirect)
{
	if (vars->cmd_index == 1 && redirect->next != NULL)
		first_command_redirection(vars, redirect);
	else if (vars->cmd_index != 1 && redirect->next != NULL)
	{
		if (vars->cmd_index % 2 == 1)
			pipe_command_redirection_odd(redirect, vars);
		else
			pipe_command_redirection_even(redirect, vars);
	}
	else if (redirect->next == NULL)
	{
		if (vars->cmd_index % 2 == 1)
			last_command_redirection_odd(redirect, vars);
		else
			last_command_redirection_even(redirect, vars);
	}
}

static t_bool	is_last_infile(t_redirection_to_expand *tmp_redirection)
{
	tmp_redirection = tmp_redirection->next;
	while (tmp_redirection)
	{
		if (tmp_redirection->e_type == REDIRECTION_HEREDOC
			|| tmp_redirection->e_type == REDIRECTION_INFILE)
			return (FALSE);
		tmp_redirection = tmp_redirection->next;
	}
	return (TRUE);
}

static void	where_are_heredoc(t_redirection **list, t_bool exist)
{
	t_redirection	*tmp;

	tmp = *list;
	while (tmp)
	{
		if (exist == FALSE)
			tmp->e_position = NONE;
		else if (exist == TRUE && tmp->e_position != HERE)
			tmp->e_position = COMING;
		tmp = tmp->next;
	}
}

t_redirection	*stock_redirection(t_command_to_expand *list)
{
	t_command_to_expand		*tmp_command;
	t_redirection_to_expand	*tmp_redirection;
	t_redirection			*redirection;
	t_redirection			*result;
	t_bool					heredoc;
	int	save;

	tmp_command = list;
	heredoc = FALSE;
	result = NULL;
	// Why ??????
	// si fd = -1 exit code 1 
	if (tmp_command)
	{
		while (tmp_command)
		{
			redirection = ft_lstnew_redirection();
			if (!redirection)
				return (NULL);
			tmp_redirection = tmp_command->redirections;
			save = 0;
			while (tmp_redirection)
			{
				if (tmp_redirection->e_type == REDIRECTION_OUTFILE)
				{
					if (redirection->outfile_fd != STDOUT_FILENO
						&& redirection->outfile_fd != -1)
					{
						close(redirection->outfile_fd);
						redirection->outfile_fd = -1;
					}
					redirection->outfile_fd = open(tmp_redirection->arg, O_CREAT | O_TRUNC | O_WRONLY, 0644);
				}
				if (tmp_redirection->e_type == REDIRECTION_INFILE)
				{
					if (redirection->infile_fd != STDIN_FILENO
						&& redirection->infile_fd != -1)
					{
						close(redirection->infile_fd);
						redirection->infile_fd = -1;
					}
					redirection->infile_fd = open(tmp_redirection->arg, O_RDONLY, 0644);
				}
				if (tmp_redirection->e_type == REDIRECTION_APPEND)
				{
					if (redirection->outfile_fd != STDOUT_FILENO
						&& redirection->outfile_fd != -1)
					{
						close(redirection->outfile_fd);
						redirection->outfile_fd = -1;
					}
					redirection->outfile_fd = open(tmp_redirection->arg, O_CREAT | O_WRONLY, 0644);
				}
				if (tmp_redirection->e_type == REDIRECTION_HEREDOC
					&& is_last(tmp_redirection) == tmp_redirection)
				{
					if (is_last_infile(tmp_redirection) == TRUE)
					{
						redirection->e_position = HERE;
						redirection->limiter = tmp_redirection->arg;
						if (access("/tmp/.heredoc", F_OK) == 0)
							unlink("/tmp/.heredoc");
						redirection->infile_fd = open("/tmp/.heredoc", O_WRONLY | O_CREAT , 0644);
						if (redirection->infile_fd != -1)
						{
							ft_heredoc(redirection, TRUE);
							close(redirection->infile_fd);
							redirection->infile_fd = -1;
							redirection->infile_fd = open("/tmp/.heredoc", O_RDONLY, 0644);
						}
						else
							perror("Error opening heredoc files");
					}
					else
						ft_heredoc(redirection, FALSE);
					heredoc = TRUE;
				}
				tmp_redirection = tmp_redirection->next;
				save++;
			}
			if (tmp_command->next != NULL)
				redirection->e_type = PIPE_OUT;
			if (tmp_redirection == NULL && save == 0)
			{
				result = ft_lstnew_redirection();
				if (!result)
					return (NULL);
			}
			where_are_heredoc(&redirection, heredoc);
			ft_lstadd_back_redirection(&result, redirection);
			tmp_command = tmp_command->next;
		}
	}

	// exit code 0 si tout se passe bien
	return (result);
}
