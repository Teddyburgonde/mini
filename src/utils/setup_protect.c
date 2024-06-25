/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_protect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:16:31 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/24 15:17:35 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	security_clear(t_command_to_expand **list)
{
	t_command_to_expand	*tmp_command;

	tmp_command = *list;
	while (tmp_command)
	{
		ft_lstclear_redirections(&tmp_command->redirections);
		tmp_command = tmp_command->next;
	}
}

void	redirection_setup(t_redirection **redirection, int nb_hd)
{
	*redirection = ft_lstnew_redirection();
	if (!redirection)
	{
		*redirection = NULL;
		return ;
	}
	(*redirection)->name_infile = NULL;
	(*redirection)->name_outfile = NULL;
	(*redirection)->nb_heredoc = nb_hd;
}

void	init(t_bool	*heredoc, int *nb_hd, t_redirection **result,
	t_redirection **redirection)
{
	*heredoc = FALSE;
	*nb_hd = 0;
	*redirection = NULL;
	*result = NULL;
}
