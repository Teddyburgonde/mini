/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:11:14 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/18 10:35:15 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"
#include <unistd.h>

void	fill_tmp_content(char **tmp_content, t_redirection *redirection, int fd_tmp)
{
	t_redirection *tmp_redirection;
	tmp_redirection = redirection;

	*tmp_content = readline("> ");
	if (*tmp_content == NULL)
	{
		free(*tmp_content);
		ft_putstr_fd("bash: warning: here-document at line 5 delimited by end-of-file (wanted `:')", 2);
		return ;
	}
	if (ft_strcmp(*tmp_content, tmp_redirection->limiter) == 0)
	{
		free(*tmp_content);
		*tmp_content = NULL;
		//exit (500);
		// close(fd_tmp);
		return ;
	}
	else
	{
		ft_putstr_fd(*tmp_content, fd_tmp);
		ft_putstr_fd("\n", fd_tmp);
	}
}

void	ft_heredoc(t_redirection *redirection,
		t_redirection_to_expand *all, t_bool save)
{
	char			*tmp_content;
	int				count;

	tmp_content = NULL;
	while (all && save == TRUE)
	{
		if (all->e_type == REDIRECTION_HEREDOC)
		{
			count = 0;
			while (ft_strcmp(tmp_content, all->arg) != 0 || count == 0)
			{
				if (redirection->e_position == HERE && save == TRUE && is_last(all) == all)
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
				else
				{
					if (tmp_content)
						free(tmp_content);
					tmp_content = NULL;
					tmp_content = readline("> ");
					if (tmp_content == NULL)
					{
						free(tmp_content);
						ft_putstr_fd("bash: warning: here-document at line 5 delimited by end-of-file (wanted `:')", 2);
						break ;
					}
				}
				count++;
			}
			free(tmp_content);
			tmp_content = NULL;
		}
		all = all->next;
	}
}

