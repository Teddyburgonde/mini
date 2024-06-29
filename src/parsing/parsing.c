/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:31:02 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/29 20:18:03 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_command_parsing_result	*parse_command(char *command_line)
{
	t_command_parsing_result		*result;
	t_redirection_parsing			*redirection_result;
	t_argument_parsing				*argument_result;
	char							*remaining_line;

	initialize_allocated_parse_command(&result,
		&redirection_result, &argument_result);
	remaining_line = skip_spaces(command_line);
	if (check_remaining_line_length(remaining_line, result) == 1)
		return (result);
	while (remaining_line && ft_strlen(remaining_line) > 0
		&& remaining_line[0] != '|')
	{
		result = ft_redirections_arguments(&remaining_line, result,
				redirection_result, argument_result);
		if (result->did_succeed == FALSE)
		{
			ft_lstclear_redirections(&result->command->redirections);
			return (result);
		}
	}
	result->remaining_line = remaining_line;
	result->did_succeed = TRUE;
	return (result);
}

t_command_line_parsing	*parsing_command(
		char *remaining_line, t_command_line_parsing *result
		, t_command_parsing_result *command_parsing)
{
	while (ft_strlen(remaining_line) > 0)
	{
		command_parsing = parse_command(remaining_line);
		if (verif_command_parsing(command_parsing) == 1)
		{
			result->did_succeed = FALSE;
			return (result);
		}
		ft_command_to_expand_addback(
			&result->commands, command_parsing->command);
		remaining_line = (char *)command_parsing->remaining_line;
		remaining_line = skip_spaces(remaining_line);
		if (pipes_management(&remaining_line) == 1)
		{
			result->did_succeed = FALSE;
			return (result);
		}
		free(command_parsing);
	}
	result->did_succeed = TRUE;
	return (result);
}

t_command_line_parsing	*ft_parse_command_line(char *command_line)
{
	char							*remaining_line;
	t_command_line_parsing			*result;
	t_command_parsing_result		*command_parsing;

	result = malloc(sizeof(t_command_line_parsing));
	if (!result)
		return (NULL);
	result->commands = NULL;
	command_parsing = NULL;
	remaining_line = skip_spaces(command_line);
	if (ft_strlen(remaining_line) == 0)
	{
		result->did_succeed = TRUE;
		return (result);
	}
	result = parsing_command(remaining_line, result, command_parsing);
	if (result->did_succeed == FALSE)
		return (result);
	result->did_succeed = TRUE;
	return (result);
}
