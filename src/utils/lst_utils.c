/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:01:08 by rgobet            #+#    #+#             */
/*   Updated: 2024/04/20 11:11:20 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_lstclear_redirections(t_redirection_to_expand **lst)
{
	t_redirection_to_expand	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		free(tmp->arg);
		free(tmp);
	}
}

void	ft_lstclear_arguments(t_argument_to_expand **lst)
{
	t_argument_to_expand	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		free(tmp->content);
		free(tmp);
	}
}

void	ft_lstclear_commands(t_command_to_expand **lst)
{
	t_command_to_expand	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		ft_lstclear_redirections(&tmp->redirections);
		ft_lstclear_arguments(&tmp->arguments);
		free(tmp);
	}
}
