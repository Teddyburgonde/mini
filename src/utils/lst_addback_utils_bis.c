/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_addback_utils_bis.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:07:59 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/20 12:52:10 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_lstadd_back_char_list(t_char_list **lst, t_char_list *new)
{
	t_char_list	*tmp;

	new->next = NULL;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp && tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
}

void	ft_lstadd_back_argument(t_argument **lst, t_argument *new)
{
	t_argument	*tmp;

	new->next = NULL;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp && tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
}

void	ft_lstadd_back_redirection(t_redirection **lst, t_redirection *new)
{
	t_redirection	*ptr_last_element;

	new->next = NULL;
	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ptr_last_element = ft_lstlast_redirection(*lst);
	ptr_last_element->next = new;
}
