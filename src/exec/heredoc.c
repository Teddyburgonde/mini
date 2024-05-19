/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:11:14 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/19 16:37:19 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"

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

static t_heredoc	*get_heredoc(t_redirection *tmp_redirection)
{
	t_heredoc	*result;
	t_heredoc	*tmp;

	while (ft_strcmp(tmp->arg, tmp_redirection->limiter) != 0)
	{
		tmp = ft_lstnew_heredoc();
		if (!tmp)
			return (NULL);
		tmp->arg = get_next_line(STDIN_FILENO);
		ft_lstadd_back_heredoc(&result, tmp);

	}
	return (result);
}

t_heredoc	*ft_heredoc(t_redirection *redirection)
{
	t_heredoc		*result;
	t_redirection	*tmp_redirection;
	char	*tmp;
	int	fd;

	tmp_redirection = redirection;
	tmp = NULL;
	while (tmp_redirection)
	{
		if (tmp_redirection->e_position == HERE)
			result = get_heredoc(tmp_redirection);
		while (ft_strcmp(tmp, tmp_redirection->limiter) != 0)
		{
			if (tmp)
				free(tmp);
			if (tmp_redirection->e_position == HERE)
				break ;
			tmp = get_next_line(STDIN_FILENO);
		}
		free(tmp);
		tmp_redirection = tmp_redirection->next;
	}
	return (result);
}
