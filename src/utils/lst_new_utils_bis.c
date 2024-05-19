/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_new_utils_bis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:02:42 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/19 15:47:59 by tebandam         ###   ########.fr       */
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
	tmp->e_type = STDOUT;
	tmp->infile_fd = STDIN_FILENO;
	tmp->outfile_fd = STDOUT_FILENO;
	tmp->limiter = NULL;
	tmp->next = NULL;
	return (tmp);
}

t_heredoc	*ft_lstnew_heredoc(void)
{
	t_heredoc	*tmp;

	tmp = (t_heredoc *)malloc(sizeof(t_heredoc));
	if (!tmp)
		return (NULL);
	tmp->arg = NULL;
	tmp->next = NULL;
	return (tmp);
}
