/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:01:08 by rgobet            #+#    #+#             */
/*   Updated: 2024/04/26 22:55:44 by rgobet           ###   ########.fr       */
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
		if (tmp->arg)
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
		if (tmp->content)
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
		if (tmp->redirections)
			ft_lstclear_redirections(&tmp->redirections);
		if (tmp->arguments)
			ft_lstclear_arguments(&tmp->arguments);
		free(tmp);
	}
}

// int	ft_lstsize_expand(t_char_list *lst)
// {
// 	t_char_list	*tmp;
// 	t_bool		in_quote;
// 	int			size;

// 	tmp = lst;
// 	size = 0;
// 	if (tmp->value == '\'' || tmp->value == '"')
// 	{
// 		size++;
// 		in_quote = TRUE;
// 		tmp = tmp->next;
// 	}
// 	else
// 		in_quote = FALSE;
// 	while (tmp && in_quote == FALSE)
// 	{
// 		if (tmp->value == SPACE || tmp->value == TAB || tmp->value == NEW_LINE)
// 			break ;
// 		else if (tmp->value == '\'' || tmp->value == '"')
// 			break ;
// 		else
// 		{
// 			tmp = tmp->next;
// 			size++;
// 		}
// 	}
// 	while (tmp && in_quote == TRUE)
// 	{
// 		if (tmp->value == '\'' || tmp->value == '"')
// 		{
// 			in_quote = FALSE;
// 			size++;
// 		}
// 		else
// 		{
// 			tmp = tmp->next;
// 			size++;
// 		}
// 	}
// 	return (size);
// }

void	ft_lstclear_char_list(t_char_list **lst)
{
	t_char_list	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		free(tmp);
	}
}

void	ft_lstclear_argument(t_argument **lst)
{
	t_argument	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		if (tmp->chars)
			ft_lstclear_char_list(&tmp->chars);
		free(tmp);
	}
}
