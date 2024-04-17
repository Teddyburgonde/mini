/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:31:02 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/17 17:11:21 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


// ici ne pas enlever * sur result 
t_argument_parsing_result *parse_quote(const char *remaining_line, t_argument_parsing_result *result)
{
	if (remaining_line[0] == '\'')
	{
		result->argument->content = ft_strjoin_quoted_arg(
				result->argument->content, remaining_line, "\'");
		remaining_line = skip_quote(remaining_line, '\'', result);
		if (result->did_succeed == FALSE)
			return (result);
	}
	else if (remaining_line[0] == '"')
	{
		result->argument->content = ft_strjoin_quoted_arg(
				result->argument->content, remaining_line, "\"");
		remaining_line = skip_quote(remaining_line, '"', result);
		if (result->did_succeed == FALSE)
			return (result);
	}
	result->remaining_line = remaining_line;
	return (result);
}
// ici ne pas enlever * sur result 
t_argument_parsing_result *is_parsing_arg(const char *remaining_line, t_argument_parsing_result *result)
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
			continue;
		}
		result->argument->content = ft_strjoin_arg(
			result->argument->content, remaining_line);
		remaining_line = ft_skip_arg(remaining_line, "<>\'\"| \n\t");
	}
	result->remaining_line = remaining_line;
	return (result);
}
// ici ne pas enlever * sur result 
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
// ici ne pas enlever * sur result 
t_redirection_parsing_result *ft_verif_redirection(char *str, t_redirection_parsing_result *redirection_result)
{
	if (str[0] == '>' && str[1] == '>')
	{
		if (double_redirection(str))
			redirection_result->did_succeed = FALSE;
		redirection_result->redirection->type = REDIRECTION_APPEND;
	}
	else if (str[0] == '<' && str[1] == '<')
	{
		if (double_redirection(str))
			redirection_result->did_succeed = FALSE;
		redirection_result->redirection->type = REDIRECTION_HEREDOC;
	}
	else if (str[0] == '<')
	{
		if (single_redirection(str))
			redirection_result->did_succeed = FALSE;
		redirection_result->redirection->type = REDIRECTION_INFILE;
	}
	else if (str[0] == '>')
	{
		if (single_redirection(str))
			redirection_result->did_succeed = FALSE;
		redirection_result->redirection->type = REDIRECTION_OUTFILE;
	}
	return (redirection_result);
}
// ici ne pas enlever * sur result 
t_redirection_parsing_result	*parse_redirection(char *str)
{
	t_redirection_parsing_result	*redirection_result;

	redirection_result = malloc(sizeof(t_redirection_parsing_result));
	if (!redirection_result)
		return (NULL);
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
	redirection_result->redirection->arg = ft_strjoin_arg(NULL, redirection_result->redirection->arg);
	redirection_result->remaining_line = str;
	return (redirection_result);
}

t_command_parsing_result	*parse_command(char *command_line)
{
	t_command_parsing_result		*result;
	t_redirection_parsing_result	*redirection_result;
	t_argument_parsing_result		*argument_result;
	char							*remaining_line;

	result = malloc(sizeof(t_command_parsing_result));
	result->command = malloc(sizeof(t_command_to_expand));
	result->remaining_line = NULL;
	remaining_line = skip_spaces(command_line);
	result->command->arguments = NULL;
	result->command->redirections = NULL;
	if (ft_strlen(remaining_line) == 0)
	{
		result->did_succeed = TRUE;
		return (result);
	}
	// help sperateur , voir avec thibaut
	// echo kebab$LANGUAGE
	// kebaben
	while (remaining_line && ft_strlen(remaining_line) > 0
		&& remaining_line[0] != '|')
	{
		if (remaining_line[0] == '>' || remaining_line[0] == '<')
		{
			redirection_result = parse_redirection(remaining_line);
			if (redirection_result->did_succeed != TRUE)
			{
				result->did_succeed = FALSE;
				return (result);
			}
			ft_redirection_to_expand_addback(
				&result->command->redirections, redirection_result->redirection);
		}
		else
		{
			argument_result = parse_argument(remaining_line);
			if (argument_result->did_succeed != TRUE)
			{
				result->did_succeed = FALSE;
				return (result);
			}
			ft_argument_to_expand_addback(&result->command->arguments,
				argument_result->argument);
		}
		remaining_line = (char*)argument_result->remaining_line;
		remaining_line = skip_spaces(remaining_line);
	}
	result->remaining_line = remaining_line;
	result->did_succeed = TRUE;
	return (result);
}
// normer 
// verif sur les malloc 
// eviter les mallocs sur les results (enlever le *)
// blou bloup | bili | boup |
//==45406== Conditional jump or move depends on uninitialised value(s)
// ==45406==    at 0x4026BE: ft_lstlast (lst_addback_utils.c:67)
// ==45406==    by 0x402681: ft_command_to_expand_addback (lst_addback_utils.c:82)
// ==45406==    by 0x401E52: ft_parse_command_line (parsing.c:212)
// ==45406==    by 0x40133D: ft_readline (ft_readline.c:32)
// ==45406==    by 0x401277: main (minishell.c:33)
// ==45406== 

t_command_line_parsing_result	*ft_parse_command_line(char *command_line)
{
	char							*remaining_line;
	t_command_line_parsing_result	*result;
	t_command_parsing_result		*command_parsing_result;

	result = malloc(sizeof(t_command_line_parsing_result));
	if (!result)
		return (NULL);
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
		if (command_parsing_result == NULL)
		{
			result->did_succeed = FALSE;
			return result;
			
		}
		printf("CMD\n");
		printf("%s\n", command_parsing_result->command->arguments->content);
		if (!command_parsing_result->did_succeed)
		{
			result->did_succeed = FALSE;
			return (result);
		}
		ft_command_to_expand_addback(
			&result->commands, command_parsing_result->command);
		remaining_line = (char *)command_parsing_result->remaining_line;
		remaining_line = skip_spaces(remaining_line);
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
	result->did_succeed = TRUE;
	return (result);
}
