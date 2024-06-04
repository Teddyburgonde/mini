/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:11:14 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/04 12:01:15 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"
#include <unistd.h>

// void	ft_get_nb_hd(t_vars *vars, t_redirection_to_expand *redir)
// {
// 	int	count;

// 	count = 0;
// 	while (redir)
// 	{
// 		if (redir->e_type == REDIRECTION_HEREDOC)
// 			count++;
// 		redir = redir->next;
// 	}
// 	vars->nb_hd = count;
// }

// void	ft_heredoc(t_vars *vars, t_redirection_to_expand *redir)
// {
// 	char	*tab;
// 	int		i;

// 	i = 0;
// 	tab = NULL;
// 	ft_get_nb_hd(vars, redir);
// 	open_hd_w(vars);
// 	while (redir && vars->nb_hd > 0)
// 	{
// 		while (redir->e_type != REDIRECTION_HEREDOC)
// 			redir = redir->next;
// 		write(1, "> ", 2);
// 		tab = get_next_line(STDIN_FILENO);
// 		verif_tab(tab);
// 		capture_and_redirection(tab, redir->arg, vars);
// 		redir = redir->next;
// 		vars->nb_hd--;
// 	}
// 	close(vars->hd_w);
// 	open_fd_infile(vars);
// }




void	fill_tmp_content(char **tmp_content, t_redirection *redirection, int fd_tmp)
{
	t_redirection *tmp_redirection;
	tmp_redirection = redirection;

	*tmp_content = readline("> ");
	if (*tmp_content == NULL)
	{
		perror("Error readline\n");
		return ;
	}
	if (ft_strcmp(*tmp_content, tmp_redirection->limiter) == 0)
	{
		free(*tmp_content);
		*tmp_content = NULL;
		// close(fd_tmp);
		// unlink("tmp_heredoc");
		return ;
	}
	else
	{
		ft_putstr_fd(*tmp_content, fd_tmp);
		ft_putstr_fd("\n", fd_tmp);
	}
}

// void	open_fd_tmp_for_heredoc(int *fd_tmp)
// {
// 	*fd_tmp = open("tmp_heredoc", O_TRUNC | O_CREAT | O_WRONLY, 0644);
// 	if (*fd_tmp < 0)
// 	{
// 		perror("Error opening file\n");
// 		return ;
// 	}
// }

void	ft_heredoc(t_redirection *redirection, t_bool save)
{
	t_redirection	*tmp_redirection;
	char			*tmp_content;

	tmp_content = NULL;
	tmp_redirection = redirection;
	if (tmp_redirection->e_position == HERE && save == TRUE)
	{
		// open_fd_tmp_for_heredoc(&fd_tmp);
		// if (fd_tmp < 0)
		// 	return ;
		while (1)
		{
			fill_tmp_content(&tmp_content, redirection, redirection->infile_fd);
			if (tmp_content == NULL)
				return ;
			else
				free(tmp_content);
		}
		// close(fd_tmp);
	}
}
