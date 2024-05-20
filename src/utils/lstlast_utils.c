/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstlast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 15:49:52 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/20 12:33:22 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_redirection	*ft_lstlast_redirection(t_redirection *lst)
{
	t_redirection	*ptr_last_element;

	if (!lst)
		return (NULL);
	ptr_last_element = lst;
	while (ptr_last_element->next != NULL)
	{
		ptr_last_element = ptr_last_element->next;
	}
	return (ptr_last_element);
}
