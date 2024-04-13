/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:31:02 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/13 14:51:31 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_argument_parsing_result *parse_argument(char *command_line)
{
	char						*remaining_line;
	t_argument_parsing_result	*result;

	result = malloc(sizeof(t_argument_parsing_result));
	result->argument = lst_new_argument_parsing_result();
	result->did_succeed = TRUE;
	remaining_line = skip_spaces(command_line);
	while (remaining_line[0] != '|' && remaining_line[0] != '<'
		&& remaining_line[0] != '>' && remaining_line[0] != ' '
		&& remaining_line[0] != '\n' && remaining_line[0] != '\0'
		&& remaining_line[0] != '\t')
	{
		if (remaining_line[0] == '\'')
		{
			result->argument->content = ft_strjoin_until(
					result->argument->content, remaining_line, '\'');
			skip_quote(remaining_line, '\'', result);
			if (result->did_succeed == FALSE)
				return (result);
			break ;
		}
		else if (remaining_line[0] == '"')
		{
			result->argument->content = ft_strjoin_until(
					result->argument->content, remaining_line, '"');
			skip_quote(remaining_line, '"', result);
			if (result->did_succeed == FALSE)
				return (result);
			break ;
		}
		else
		{
			result->argument->content = ft_strjoin_arg(
					result->argument->content, remaining_line);
			break ;
		}
	}
	result->remaining_line = remaining_line;
	return (result);
}

t_redirection_parsing_result	parse_redirection(char *str)
{
	t_redirection_parsing_result	redirection_result;
	char							*remaining_line;

	remaining_line = str;
	skip_one_character(str);
	skip_spaces(str);
	redirection_result.redirection = lst_new_redirection_parsing_result();
	// Manage errors
	while (remaining_line[0] != '|' && remaining_line[0] != '<'
		&& remaining_line[0] != '>' && remaining_line[0] != ' '
		&& remaining_line[0] != '\n' && remaining_line[0] != '\0'
		&& remaining_line[0] != '\t')
	{
		if (str[0] == '"')
		{
			redirection_result.redirection->arg = ft_strjoin_until(
					redirection_result.redirection->arg, str, '"');
			break ;
		}
		else if (str[0] == '\'')
		{
			redirection_result.redirection->arg = ft_strjoin_until(
					redirection_result.redirection->arg, str, '\'');
			break ;
		}
		else
		{
			redirection_result.redirection->arg = ft_strjoin_arg(
					redirection_result.redirection->arg, str);
			break ;
		}
	}
	redirection_result.remaining_line = str;
	return (redirection_result);
}

t_command_parsing_result *parse_command(char *command_line)
{
	t_command_parsing_result		*result;
	t_redirection_parsing_result	redirection_result;
	t_argument_parsing_result		*argument_result;
	char							*remaining_line;

	result = malloc(sizeof(t_command_parsing_result));
	result->command = malloc(sizeof(t_command_to_expand));
	result->remaining_line = NULL;
	remaining_line = skip_spaces(command_line);
	result->command->arguments = NULL;
	if (ft_strlen(remaining_line) == 0)
	{
		result->did_succeed = TRUE;
		return (result);
	}

	while (remaining_line && ft_strlen(remaining_line) > 0 && remaining_line[0] != '|')
	{
		printf("debog [%c]\n", *remaining_line);
		if (remaining_line[0] == '>' || remaining_line[0] == '<')
		{
			redirection_result = parse_redirection(remaining_line);
			if (redirection_result.did_succeed != TRUE)
			{
				result->did_succeed = FALSE;
				return (result);
			}
			ft_redirection_to_expand_addback(
				&result->command->redirections, redirection_result.redirection);
		}
		else
		{
			argument_result = parse_argument(remaining_line);
			if (argument_result->did_succeed != TRUE)
			{
				result->did_succeed = FALSE;
				return (result);
			}
			ft_argument_to_expand_addback(&result->command->arguments, argument_result->argument);
		}
		remaining_line = result->remaining_line;
		remaining_line = skip_spaces(remaining_line);
	}
	result->did_succeed = TRUE;
	return (result);
}

t_command_line_parsing_result *ft_parse_command_line(char *command_line)
{
	char							*remaining_line;
	t_command_line_parsing_result	*result;
	t_command_parsing_result		*command_parsing_result;

	result = malloc(sizeof(t_command_line_parsing_result));
	result->commands = NULL;
	remaining_line = skip_spaces(command_line);
	if (ft_strlen(remaining_line) == 0)
	{
		result->did_succeed = TRUE;
		return (result);
	}
	while (ft_strlen(remaining_line) > 0)
	{
		command_parsing_result = parse_command(remaining_line);
		if (!command_parsing_result->did_succeed)
		{
			result->did_succeed = FALSE;
			return (result);
		}
		ft_command_to_expand_addback(
			&result->commands, command_parsing_result->command);

		remaining_line = command_parsing_result->remaining_line;
		remaining_line = skip_spaces(remaining_line);

		if (remaining_line[0] == '|')
		{
			remaining_line = skip_one_character(remaining_line);
			remaining_line = skip_spaces(remaining_line);

			if (ft_strlen(remaining_line) == 0)
			{
				result->did_succeed = FALSE;
				return (result);
			}
		}
	}
	result->did_succeed = TRUE;
	return (result);
}
