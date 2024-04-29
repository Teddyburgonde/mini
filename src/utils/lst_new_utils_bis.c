/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_new_utils_bis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:02:42 by rgobet            #+#    #+#             */
/*   Updated: 2024/04/28 15:17:45 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_char_list	*lst_new_char_list(void)
{
	t_char_list	*tmp;

	tmp = malloc(sizeof(t_char_list));
	if (!tmp)
		return (NULL);
	tmp->value = 0;
	tmp->last_pos = FALSE;
	tmp->was_in_a_variable = FALSE;
	tmp->next = NULL;
	return (tmp);
}

t_argument	*lst_new_argument(void)
{
	t_argument	*tmp;

	tmp = malloc(sizeof(t_argument));
	if (!tmp)
		return (NULL);
	tmp->chars = NULL;
	tmp->next = NULL;
	return (tmp);
}
