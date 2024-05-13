/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_addback_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 11:03:46 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/11 15:37:58 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*tmp;

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

void	ft_argument_to_expand_addback(
	t_argument_to_expand **lst, t_argument_to_expand *new)
{
	t_argument_to_expand	*ptr_last_element;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ptr_last_element = *lst;
	while (ptr_last_element->next != NULL)
		ptr_last_element = ptr_last_element->next;
	ptr_last_element->next = new;
}

void	ft_redirection_to_expand_addback(
	t_redirection_to_expand **lst, t_redirection_to_expand *new)
{
	t_redirection_to_expand	*ptr_last_element;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ptr_last_element = *lst;
	while (ptr_last_element->next != NULL)
		ptr_last_element = ptr_last_element->next;
	ptr_last_element->next = new;
}

static t_command_to_expand	*ft_lstlast(t_command_to_expand *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_command_to_expand_addback(
	t_command_to_expand **lst, t_command_to_expand *new)
{
	new->next = NULL;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	ft_lstlast(*lst)->next = new;
}
