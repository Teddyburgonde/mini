/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:01:08 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/25 16:31:32 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	close_file_descriptors(t_redirection *node)
{
	if (node->infile_fd == STDIN_FILENO || node->infile_fd == STDOUT_FILENO)
		node->infile_fd = -1;
	if (node->outfile_fd == STDIN_FILENO || node->outfile_fd == STDOUT_FILENO)
		node->outfile_fd = -1;
	if (node->outfile_fd != -1)
	{
		close(node->outfile_fd);
		node->outfile_fd = -1;
	}
	if (node->infile_fd != -1)
	{
		close(node->infile_fd);
		node->infile_fd = -1;
	}
}

static void	free_node_resources(t_redirection *node)
{
	if (node->name_outfile)
		free(node->name_outfile);
	if (node->name_infile)
		free(node->name_infile);
	if (node->e_position == HERE)
		unlink(node->file_heredoc);
	if (node->limiter)
		free(node->limiter);
	if (node->file_heredoc)
		free(node->file_heredoc);
}

static void	clear_node(t_redirection *node)
{
	if (!node)
		return ;
	close_file_descriptors(node);
	free_node_resources(node);
	free(node);
}

void	ft_lstclear_final_redirection(t_redirection **lst)
{
	t_redirection	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		clear_node(tmp);
	}
}
