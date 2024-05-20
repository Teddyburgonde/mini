/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:11:14 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/20 12:58:29 by tebandam         ###   ########.fr       */
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

t_redirection_to_expand	*is_last(t_command_to_expand *cmd)
{
	t_redirection_to_expand	*result;
	t_redirection_to_expand	*tmp;

	result = NULL;
	while (cmd)
	{
		tmp = cmd->redirections;
		while (tmp)
		{
			if (tmp->e_type == REDIRECTION_HEREDOC)
				result = tmp;
			tmp = tmp->next;
		}
		cmd = cmd->next;
	}
	return (result);
}

static void	get_heredoc(t_redirection *tmp_redirection)
{
	char	*buffer;

	buffer = NULL;
	while (ft_strcmp(buffer, tmp_redirection->limiter) != 0)
	{
		if (buffer)
			free(buffer);
		write(1, "> ", 2);
		buffer = get_next_line(STDIN_FILENO);
		write(tmp_redirection->infile_fd, buffer, ft_strlen(buffer));
	}
	if (buffer)
		free(buffer);
}

void	ft_heredoc(t_redirection *redirection, t_bool save)
{
	t_redirection	*tmp_redirection;
	char	*tmp;

	tmp_redirection = redirection;
	tmp = NULL;
	while (tmp_redirection)
	{
		if (tmp_redirection->e_position == HERE && save == TRUE)
			get_heredoc(tmp_redirection);
		while (ft_strcmp(tmp, tmp_redirection->limiter) != 0)
		{
			if (tmp)
				free(tmp);
			if (tmp_redirection->e_position == HERE)
				break ;
			write(1, "> ", 2);
			tmp = get_next_line(STDIN_FILENO);
		}
		if (tmp)
			free(tmp);
		tmp_redirection = tmp_redirection->next;
	}
}
