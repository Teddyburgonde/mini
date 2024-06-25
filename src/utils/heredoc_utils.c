/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:59:39 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/25 13:27:24 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

void	where_are_heredoc(t_redirection **list, t_bool exist)
{
	t_redirection	*tmp;

	tmp = *list;
	while (tmp)
	{
		if (exist == FALSE && tmp)
			tmp->e_position = NONE;
		else if (exist == TRUE && tmp && tmp->e_position != HERE)
			tmp->e_position = COMING;
		tmp = tmp->next;
	}
}

void	prepare_heredoc(t_redirection *redirection,
	t_redirection_to_expand *tmp_redirection)
{
	redirection->nb_heredoc++;
	redirection->e_position = HERE;
	redirection->limiter = copy(tmp_redirection->arg);
	redirection->file_heredoc = ft_itoa(
			redirection->nb_heredoc);
	redirection->file_heredoc = ft_strjoin_free_s2(
			"/tmp/.heredoc", redirection->file_heredoc);
	if (access(redirection->file_heredoc, F_OK) == 0)
		unlink(redirection->file_heredoc);
	redirection->infile_fd = open(redirection->file_heredoc,
			O_WRONLY | O_TRUNC | O_CREAT, 0644);
}

void	heredoc_setup(t_redirection *redirection,
	t_command_to_expand *tmp_command, t_env *env, t_vars *vars)
{
	ft_heredoc(redirection,
		tmp_command->redirections, env, vars);
	close(redirection->infile_fd);
	redirection->infile_fd = -1;
	redirection->infile_fd = open(
			redirection->file_heredoc, O_RDONLY, 0644);
}

void	complete_heredoc(t_redirection *redirection,
	t_command_to_expand *tmp_command, t_env *env, t_vars *vars)
{
	if (redirection->nb_heredoc && redirection->infile_fd != -1)
		heredoc_setup(redirection, tmp_command, env, vars);
	else
		perror("Error opening heredoc files");
}
