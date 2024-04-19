/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:59:29 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/19 16:54:49 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_argument_parsing_result	*is_parsing_arg(const char *remaining_line,
	t_argument_parsing_result *result)
{
	while (remaining_line[0] != '|' && remaining_line[0] != '<'
		&& remaining_line[0] != '>' && remaining_line[0] != ' '
		&& remaining_line[0] != '\n' && remaining_line[0] != '\0'
		&& remaining_line[0] != '\t')
	{
		if (remaining_line[0] == '\'' || remaining_line[0] == '"')
		{
			result = parse_quote(remaining_line, result);
			remaining_line = result->remaining_line;
			if (result->did_succeed == FALSE)
				return (result);
			continue ;
		}
		result->argument->content = ft_strjoin_arg(
				result->argument->content, remaining_line);
		remaining_line = ft_skip_arg(remaining_line, "<>\'\"| \n\t");
	}
	result->remaining_line = remaining_line;
	return (result);
}

t_argument_parsing_result	*parse_argument(const char *command_line)
{
	const char					*remaining_line;
	t_argument_parsing_result	*result;

	result = malloc(sizeof(t_argument_parsing_result));
	result->argument = lst_new_argument_parsing_result();
	result->did_succeed = TRUE;
	remaining_line = skip_spaces(command_line);
	result = is_parsing_arg(remaining_line, result);
	remaining_line = result->remaining_line;
	return (result);
}

t_command_parsing_result	*redirections(t_command_parsing_result *result,
	char *remaining_line, t_redirection_parsing_result	*redirection_result)
{
	redirection_result = parse_redirection(remaining_line);
	if (redirection_result->did_succeed != TRUE)
	{
		result->did_succeed = FALSE;
		return (result);
	}
	ft_redirection_to_expand_addback(
		&result->command->redirections, redirection_result->redirection);
	remaining_line = (char *)redirection_result->remaining_line;
	result->remaining_line = remaining_line;
	return (result);
}

t_command_parsing_result	*arguments(t_command_parsing_result *result,
	t_argument_parsing_result *argument_result, char *remaining_line)
{
	argument_result = parse_argument(remaining_line);
	if (argument_result->did_succeed != TRUE)
	{
		result->did_succeed = FALSE;
		return (result);
	}
	ft_argument_to_expand_addback(&result->command->arguments,
		argument_result->argument);
	remaining_line = (char *)argument_result->remaining_line;
	result->remaining_line = remaining_line;
	return (result);
}

t_command_parsing_result	*ft_allocated_result(void)
{
	t_command_parsing_result		*result;

	result = malloc(sizeof(t_command_parsing_result));
	result->command = malloc(sizeof(t_command_to_expand));
	if (!result || !result->command)
	{
		if (!result)
			free(result);
		result->did_succeed = FALSE;
		return (result);
	}
	result->did_succeed = TRUE;
	result->remaining_line = NULL;
	result->command->arguments = NULL;
	result->command->redirections = NULL;
	
	return (result);
}
