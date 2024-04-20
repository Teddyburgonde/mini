/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_new_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 11:08:24 by rgobet            #+#    #+#             */
/*   Updated: 2024/04/20 16:15:26 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_env	*ft_lstnew_env(void)
{
	t_env	*list;

	list = malloc(sizeof(t_env));
	if (!list)
	{
		write (2, "Initialize environment variable has an error.\n", 46);
		return (NULL);
	}
	list->next = NULL;
	return (list);
}

t_argument_to_expand	*lst_new_argument_parsing_result(void)
{
	t_argument_to_expand	*tmp;

	tmp = malloc(sizeof(t_argument_to_expand));
	if (!tmp)
		return (NULL);
	tmp->content = NULL;
	tmp->next = NULL;
	return (tmp);
}

t_redirection_to_expand	*lst_new_redirection_parsing_result(void)
{
	t_redirection_to_expand	*tmp;

	tmp = malloc(sizeof(t_redirection_to_expand));
	if (!tmp)
		return (NULL);
	tmp->arg = NULL;
	tmp->e_type = UNASIGNED;
	tmp->next = NULL;
	return (tmp);
}

t_command_to_expand	*lst_new_command_parsing_result(void)
{
	t_command_to_expand	*tmp;

	tmp = malloc(sizeof(t_command_to_expand));
	if (!tmp)
		return (NULL);
	tmp->arguments = NULL;
	tmp->redirections = NULL;
	tmp->next = NULL;
	return (tmp);
}
