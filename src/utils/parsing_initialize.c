/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_initialize.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 09:38:25 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/25 14:05:59 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_redirection_parsing	*initialize_redirection_parsing_result(void)
{
	t_redirection_parsing	*redirection_result;

	redirection_result = malloc(sizeof(t_redirection_parsing));
	if (!redirection_result)
	{
		redirection_result->did_succeed = FALSE;
		return (redirection_result);
	}
	redirection_result->redirection = lst_new_redirection_parsing_result();
	if (!redirection_result->redirection)
	{
		redirection_result->did_succeed = FALSE;
		free(redirection_result);
		return (NULL);
	}
	redirection_result->did_succeed = TRUE;
	return (redirection_result);
}

void	initialize_allocated_parse_command(t_command_parsing_result **result,
	t_redirection_parsing	**redirection_result,
	t_argument_parsing **argument_result)
{
	*result = ft_allocated_result();
	*redirection_result = NULL;
	*argument_result = NULL;
}
