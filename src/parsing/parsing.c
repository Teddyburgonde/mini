/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:31:02 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/02 16:50:18 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_argument_parsing_result parse_argument(char* command_line)
{
	char						*remaining_line;
	t_argument_parsing_result	result;

	char *arg = NULL;

	remaining_line = skip_spaces(command_line);
	while (remaining_line[0] != '|' && remaining_line[0] != '<'
		&& remaining_line[0] != '>' && remaining_line[0] != ' '
		&& remaining_line[0] != '\0')
	{
		if (remaining_line[0] == '\'')
		{
			// rejoindre la quote suivante sinon erreur
			// puis copier 
		}
		else if (remaining_line[0] == '\"')
		{
			// rejoindre la quote suivante sinon erreur
			// puis copier
		}
		else
		{
			//charactere normal
			// considerer strjoin ou memcpy
		}
	}
	return (result);
}

// manque parse redirection qui depend de parse command



t_command_parsing_result parse_command(const char *command_line)
{
	t_command_parsing_result		result;
	t_redirection_parsing_result	redirection_result;
	t_argument_parsing_result		argument_result;
	const char						*remaining_line;

	remaining_line = skip_spaces(command_line);
	if (ft_strlen(remaining_line) == 0)
	{
		result.did_succeed = TRUE;
		return (result);
	}

	while (ft_strlen(remaining_line) > 0 && remaining_line[0] != '|')
	{


		if (remaining_line[0] == '>' || remaining_line[0] == '<')
		{
			redirection_result = parse_redirection(remaining_line);
			if (!redirection_result.did_succeed)
			{
				result.did_succeed = FALSE;
				return result;
			}
			ft_redirection_to_expand_addback(&result.command.redirections, redirection_result.redirection);
		}
		else
		{
			argument_result =	parse_argument(remaining_line);
			if (!argument_result.did_succeed)
			{
				result.did_succeed = FALSE;
				return (result);
			}
			// addback to list of arguments
		}
		remaining_line = skip_spaces(remaining_line);
	}
	result.did_succeed = TRUE;
	return (result);
}


t_command_line_parsing_result ft_parse_command_line(const char *command_line)
{

	const char						*remaining_line;
	t_command_line_parsing_result	result;
	t_command_parsing_result 		command_parsing_result;

	result.commands = NULL;
	remaining_line = skip_spaces(command_line);
	if (ft_strlen(remaining_line) == 0)
	{
		result.did_succeed = TRUE;
		return result;
	}

	while (ft_strlen(remaining_line) > 0)
	{
		command_parsing_result = parse_command(remaining_line);
		if (!command_parsing_result.did_succeed)
		{
			result.did_succeed = FALSE;
			return (result);
		}
		ft_command_to_expand_add_back(&result.commands, &command_parsing_result.command);

		remaining_line = command_parsing_result.remaining_line;
		remaining_line = skip_spaces(remaining_line);

		if (remaining_line[0] == '|')
		{
			remaining_line = skip_one_character(remaining_line);
			remaining_line = skip_spaces(remaining_line);

			if (ft_strlen(remaining_line) == 0)
			{
				result.did_succeed = FALSE;
				return (result);
			}
		}
	}
	result.did_succeed = TRUE;
	return (result);
}
