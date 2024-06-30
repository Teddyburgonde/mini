/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:07:02 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/29 20:39:59 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	outfile_setup(t_redirection *redirection,
	t_redirection_to_expand *tmp_redirection)
{
	if (redirection->outfile_fd != STDOUT_FILENO
		&& redirection->outfile_fd != -1)
	{
		close(redirection->outfile_fd);
		redirection->outfile_fd = -1;
		if (redirection->name_outfile)
			free(redirection->name_outfile);
		redirection->name_outfile = NULL;
	}
	redirection->outfile_fd = open(tmp_redirection->arg,
			O_CREAT | O_TRUNC | O_WRONLY, 0644);
	redirection->name_outfile = ft_strdup(tmp_redirection->arg);
}

void	infile_setup(t_redirection *redirection,
	t_redirection_to_expand *tmp_redirection)
{
	if (redirection->infile_fd != STDIN_FILENO
		&& redirection->infile_fd != -1)
	{
		close(redirection->infile_fd);
		redirection->infile_fd = -1;
		if (redirection->name_infile)
			free(redirection->name_infile);
		redirection->name_infile = NULL;
	}
	redirection->infile_fd = open(
			tmp_redirection->arg, O_RDONLY, 0644);
	redirection->name_infile = ft_strdup(tmp_redirection->arg);
}

void	append_setup(t_redirection *redirection,
	t_redirection_to_expand *tmp_redirection)
{
	if (redirection->outfile_fd != STDOUT_FILENO
		&& redirection->outfile_fd != -1)
	{
		close(redirection->outfile_fd);
		redirection->outfile_fd = -1;
		if (redirection->name_outfile)
			free(redirection->name_outfile);
		redirection->name_outfile = NULL;
	}
	redirection->outfile_fd = open(tmp_redirection->arg,
			O_CREAT | O_APPEND | O_WRONLY, 0644);
	redirection->name_outfile = ft_strdup(tmp_redirection->arg);
}

t_redirection	*no_redirection(void)
{
	t_redirection	*result;

	result = NULL;
	result = ft_lstnew_redirection();
	if (!result)
		return (NULL);
	return (result);
}

t_bool	global_redirections(t_redirection *redirection,
	t_redirection_to_expand *tmp_redirection, t_vars *vars)
{
	if (tmp_redirection->arg == NULL)
		redirection->ambiguous = TRUE;
	else if (tmp_redirection->e_type == REDIRECTION_OUTFILE)
		outfile_setup(redirection, tmp_redirection);
	else if (tmp_redirection->e_type == REDIRECTION_INFILE)
		infile_setup(redirection, tmp_redirection);
	else if (tmp_redirection->e_type == REDIRECTION_APPEND)
		append_setup(redirection, tmp_redirection);
	else if (tmp_redirection->e_type == REDIRECTION_HEREDOC
		&& is_last(tmp_redirection) == tmp_redirection)
	{
		if (is_last_infile(tmp_redirection) == TRUE)
			prepare_heredoc(redirection, tmp_redirection);
		if (redirection->infile_fd != -1)
			vars->exit_code = 1;
		return (TRUE);
	}
	return (FALSE);
}
