/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:11:14 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/17 12:41:34 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../multi_pipe.h"

void	ft_get_nb_hd(t_vars *vars, t_redirection_to_expand *redir)
{
	int	count;

	count = 0;
	while (redir)
	{
		if (redir->e_type == REDIRECTION_HEREDOC)
			count++;
		redir = redir->next;
	}
	vars->nb_hd = count;
}

void	ft_heredoc(t_vars *vars, t_redirection_to_expand *redir)
{
	char	*tab;
	int		i;

	i = 0;
	tab = NULL;
	ft_get_nb_hd(vars, redir);
	open_hd_w(vars);
	while (redir && vars->nb_hd > 0)
	{
		while (redir->e_type != REDIRECTION_HEREDOC)
			redir = redir->next;
		write(1, "> ", 2);
		tab = get_next_line(STDIN_FILENO);
		verif_tab(tab);
		capture_and_redirection(tab, redir->arg, vars);
		redir = redir->next;
		vars->nb_hd--;
	}
	close(vars->hd_w);
	open_fd_infile(vars);
}
