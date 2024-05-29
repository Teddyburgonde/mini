/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:31:02 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/29 16:54:48 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"



//exit code synstax error exit code 2 
t_argument_parsing_result	*parse_quote(const char *remaining_line,
	t_argument_parsing_result *result)
{
	if (remaining_line[0] == '\'')
	{
		result->argument->content = ft_strjoin_quoted_arg(
				result->argument->content, remaining_line, "\'");
		remaining_line = skip_quote(remaining_line, '\'', result);
		result->remaining_line = remaining_line;
		if (result->did_succeed == FALSE)
			return (result);
	}
	else if (remaining_line[0] == '"')
	{
		result->argument->content = ft_strjoin_quoted_arg(
				result->argument->content, remaining_line, "\"");
		remaining_line = skip_quote(remaining_line, '"', result);
		result->remaining_line = remaining_line;
		if (result->did_succeed == FALSE)
			return (result);
	}
	result->remaining_line = remaining_line;
	return (result);
}

t_redirection_parsing_result	*ft_verif_redirection(char *str,
	t_redirection_parsing_result *redirection_result)
{
	if (str[0] == '>' && str[1] == '>')
	{
		if (double_redirection(str))
			redirection_result->did_succeed = FALSE;
		redirection_result->redirection->e_type = REDIRECTION_APPEND;
	}
	else if (str[0] == '<' && str[1] == '<')
	{
		if (double_redirection(str))
			redirection_result->did_succeed = FALSE;
		redirection_result->redirection->e_type = REDIRECTION_HEREDOC;
	}
	else if (str[0] == '<')
	{
		if (single_redirection(str))
			redirection_result->did_succeed = FALSE;
		redirection_result->redirection->e_type = REDIRECTION_INFILE;
	}
	else if (str[0] == '>')
	{
		if (single_redirection(str))
			redirection_result->did_succeed = FALSE;
		redirection_result->redirection->e_type = REDIRECTION_OUTFILE;
	}
	return (redirection_result);
}

t_redirection_parsing_result	*parse_redirection(char *str)
{
	t_redirection_parsing_result	*redirection_result;

	redirection_result = malloc(sizeof(t_redirection_parsing_result));
	if (!redirection_result)
	{
		redirection_result->did_succeed = FALSE;
		return (redirection_result);
	}
	redirection_result->did_succeed = TRUE;
	redirection_result->redirection = lst_new_redirection_parsing_result();
	redirection_result = ft_verif_redirection(str, redirection_result);
	if (redirection_result->did_succeed == FALSE)
		return (redirection_result);
	if (str[0] == '>' && str[1] == '>')
		str = skip_one_character(str);
	else if (str[0] == '<' && str[1] == '<')
		str = skip_one_character(str);
	str = skip_one_character(str);
	str = skip_spaces(str);
	redirection_result->redirection->arg = ft_strjoin_file(
			str, redirection_result->redirection->arg);
	str = ft_skip_arg(str, " \"'\n\t|><");
	redirection_result->remaining_line = str;
	return (redirection_result);
}

t_command_parsing_result	*ft_redirections_arguments(char **remaining_line,
	t_command_parsing_result *result,
	t_redirection_parsing_result *redirection_result,
	t_argument_parsing_result *argument_result)
{
	if (*remaining_line[0] == '>' || *remaining_line[0] == '<')
	{
		result = redirections(result, *remaining_line, redirection_result);
		*remaining_line = (char *)result->remaining_line;
		*remaining_line = skip_spaces(*remaining_line);
	}
	else
	{
		result = arguments(result, argument_result, *remaining_line);
		*remaining_line = (char *)result->remaining_line;
		*remaining_line = skip_spaces(*remaining_line);
	}
	return (result);
}

t_command_parsing_result	*parse_command(char *command_line)
{
	t_command_parsing_result		*result;
	t_redirection_parsing_result	*redirection_result;
	t_argument_parsing_result		*argument_result;
	char							*remaining_line;

	result = ft_allocated_result();
	redirection_result = NULL;
	argument_result = NULL;
	remaining_line = skip_spaces(command_line);
	if (ft_strlen(remaining_line) == 0)
	{
		result->did_succeed = TRUE;
		return (result);
	}
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

t_command_line_parsing_result	*parsing_command(
		char *remaining_line, t_command_line_parsing_result *result
		, t_command_parsing_result *command_parsing_result)
{
	while (ft_strlen(remaining_line) > 0)
	{
		command_parsing_result = parse_command(remaining_line);
		if (command_parsing_result == NULL
			|| !command_parsing_result->did_succeed)
		{
			ft_lstclear_commands(&command_parsing_result->command);
			free(command_parsing_result);
			result->did_succeed = FALSE;
			return (result);
		}
		ft_command_to_expand_addback(
			&result->commands, command_parsing_result->command);
		remaining_line = (char *)command_parsing_result->remaining_line;
		remaining_line = skip_spaces(remaining_line);
		free(command_parsing_result);
		if (remaining_line && remaining_line[0] == '|')
		{
			remaining_line = skip_one_character(remaining_line);
			remaining_line = skip_spaces(remaining_line);
			if (ft_strlen(remaining_line) == 0 || remaining_line[0] == '|')
			{
				write (2, "Error: too much pipe or in a wrong position !\n", 46);
				result->did_succeed = FALSE;
				return (result);
			}
		}
	}
	return (result);
}

t_command_line_parsing_result	*ft_parse_command_line(char *command_line)
{
	char							*remaining_line;
	t_command_line_parsing_result	*result;
	t_command_parsing_result		*command_parsing_result;

	result = malloc(sizeof(t_command_line_parsing_result));
	if (!result)
		return (NULL);
	result->commands = NULL;
	command_parsing_result = NULL;
	remaining_line = skip_spaces(command_line);
	if (ft_strlen(remaining_line) == 0)
	{
		result->did_succeed = TRUE;
		return (result);
	}
	result = parsing_command(remaining_line, result, command_parsing_result);
	if (result->did_succeed == FALSE)
		return (result);
	result->did_succeed = TRUE;
	return (result);
}
