/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_new_utils_bis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:02:42 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/11 12:32:14 by rgobet           ###   ########.fr       */
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

t_redirection	*ft_lstnew_redirection(void)
{
	t_redirection	*tmp;

	tmp = (t_redirection *)malloc(sizeof(t_redirection));
	if (!tmp)
		return (NULL);
	tmp->file_heredoc = NULL;
	tmp->nb_heredoc = 0;
	tmp->ambiguous = FALSE;
	tmp->e_position = NONE;
	tmp->infile_fd = STDIN_FILENO;
	tmp->outfile_fd = STDOUT_FILENO;
	tmp->limiter = NULL;
	tmp->next = NULL;
	return (tmp);
}
