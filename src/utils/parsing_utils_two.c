/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_two.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 09:23:44 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/25 14:05:59 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_redirection_parsing	*parse_redirection(char *str)
{
	t_redirection_parsing	*redirection_result;

	redirection_result = initialize_redirection_parsing_result();
	if (!redirection_result)
		return (NULL);
	str = skip_spaces(str);
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
			str);
	str = ft_skip_arg(str, " \n\t|><");
	redirection_result->remaining_line = str;
	return (redirection_result);
}

t_redirection_parsing	*ft_verif_redirection(char *str,
	t_redirection_parsing *redirection_result)
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

t_argument_parsing	*parse_quote(const char *remaining_line,
	t_argument_parsing *result)
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

t_command_parsing_result	*ft_redirections_arguments(char **remaining_line,
	t_command_parsing_result *result,
	t_redirection_parsing *redirection_result,
	t_argument_parsing *argument_result)
{
	if ((*remaining_line)[0] == '>' || (*remaining_line)[0] == '<')
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
