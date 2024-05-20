/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_manager.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:22:33 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/20 12:57:41 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// int	open_files(t_vars *vars, t_redirection_to_expand *redir)
// {
// 	t_redirection_to_expand	*tmp;
// 	t_bool					already_get;
// 	int						hd;
// 	int						outf;
// 	int						inf;
// 	int						app;

// 	hd = 0;
// 	outf = 0;
// 	inf = 0;
// 	app = 0;
// 	vars->fd_append = 0;
// 	vars->fd_infile = 0;
// 	vars->fd_outfile = 0;
// 	vars->hd_r = 0;
// 	vars->hd_w = 0;
// 	vars->e_last = UNASIGN;
// 	already_get = FALSE;
// 	tmp = redir;
// 	while (tmp)
// 	{
// 		if (tmp->e_type == REDIRECTION_HEREDOC)
// 			hd++;
// 		else if (tmp->e_type == REDIRECTION_OUTFILE)
// 		{
// 			outf++;
// 			vars->nb_out++;
// 		}
// 		else if (tmp->e_type == REDIRECTION_INFILE)
// 			inf++;
// 		else if (tmp->e_type == REDIRECTION_APPEND)
// 			app++;
// 		tmp = tmp->next;
// 	}
// 	tmp = redir;
// 	// Droit a verif
// 	while (tmp)
// 	{
// 		if (tmp->e_type == REDIRECTION_HEREDOC)
// 		{
// 			if (hd == 1)
// 			{
// 				vars->hd_r = open(tmp->arg, O_RDONLY, 0644);
// 				vars->hd_w = open(tmp->arg, O_WRONLY, 0644);
// 			}
// 			hd--;
// 		}
// 		else if (tmp->e_type == REDIRECTION_OUTFILE)
// 		{
// 			if (outf == 1)
// 				vars->fd_outfile = open(tmp->arg, O_CREAT | O_TRUNC | O_WRONLY, 0644);
// 			else
// 			{
// 				vars->tmp_fd = open(tmp->arg, O_CREAT, 0644);
// 				close(vars->tmp_fd);
// 				vars->nb_out++;
// 			}
// 			vars->e_last = OUTFILE;
// 			outf--;
// 		}
// 		else if (tmp->e_type == REDIRECTION_INFILE)
// 		{
// 			if (already_get == FALSE)
// 				vars->fd_infile = open(tmp->arg, O_RDONLY, 0644);
// 			inf--;
// 		}
// 		else if (tmp->e_type == REDIRECTION_APPEND)
// 		{
// 			if (app == 1)
// 				vars->fd_append = open(tmp->arg, O_WRONLY | O_CREAT, 0644);
// 			else
// 			{
// 				// A verif
// 				vars->tmp_fd = open(tmp->arg, O_CREAT, 0644);
// 				close(vars->tmp_fd);
// 			}
// 			vars->e_last = APPEND;
// 			app--;
// 		}
// 		tmp = tmp->next;
// 	}
// 	return (0);
// }

void	ft_close_fd(t_vars *vars, t_redirection **redirect)
{
	close(vars->tmp_fd);
	ft_lstclear_final_redirection(redirect);
	
}
// NORMALEMENT  FAIT V
// void	open_fd_infile(t_vars *vars)
// {
// 	vars->fd_infile = open("TMP_HEREDOC", O_RDONLY, 0644);
// 	if (!vars->fd_infile)
// 	{
// 		perror("Error opening files");
// 		exit(1);
// 	}
// 	unlink("TMP_HEREDOC");
// }

// void	open_hd_w(t_vars *vars)
// {
// 	vars->hd_w = open("TMP_HEREDOC", O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (vars->hd_w == -1)
// 	{
// 		perror("Error opening files");
// 		exit(1);
// 	}
// }