/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:54:32 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/19 16:31:22 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

static void	stock_heredoc(t_command_to_expand *list, t_redirection **redirect)
{
	t_redirection_to_expand *tmp_redirection;
	t_command_to_expand *tmp_command;
	t_redirection *redirection;

	tmp_command = list;
	redirection = *redirect;
	while (tmp_command)
	{
		tmp_redirection = tmp_command->redirections;
		while (tmp_redirection)
		{
			if (tmp_redirection->e_type == REDIRECTION_HEREDOC)
				redirection->limiter = tmp_redirection->arg;
			tmp_redirection = tmp_redirection->next;
		}
		tmp_command = tmp_command->next;
	}
}

t_redirection	*stock_redirection(t_command_to_expand *list)
{
	t_command_to_expand		*courant;
	t_command_to_expand		*tmp_command;
	t_redirection_to_expand	*tmp_redirection;
	t_redirection			*redirection;
	t_redirection			*result;

	tmp_command = list;
	while (tmp_command)
	{
		redirection = ft_lstnew_redirection();
		if (!redirection)
			return (NULL);
		tmp_redirection = tmp_command->redirections;
		while (tmp_redirection)
		{
			if (tmp_redirection->e_type == REDIRECTION_OUTFILE)
			{
				if (redirection->outfile_fd != STDOUT_FILENO)
					close(redirection->outfile_fd);
				redirection->outfile_fd = open(tmp_redirection->arg, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			}
			if (tmp_redirection->e_type == REDIRECTION_INFILE)
			{
				if (redirection->infile_fd != STDIN_FILENO)
					close(redirection->infile_fd);
				redirection->infile_fd = open(tmp_redirection->arg, O_RDONLY, 0644);
			}
			if (tmp_redirection->e_type == REDIRECTION_APPEND)
			{
				if (redirection->outfile_fd != STDOUT_FILENO)
					close(redirection->outfile_fd);
				redirection->outfile_fd = open(tmp_redirection->arg, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			}
			tmp_redirection = tmp_redirection->next;
		}
		if (tmp_command->next != NULL)
			redirection->e_type = PIPE_OUT;
		ft_lstadd_back_redirection(&result, redirection);
		tmp_command = tmp_command->next;
	}
	stock_heredoc(list, &result);
	return (result);
}


