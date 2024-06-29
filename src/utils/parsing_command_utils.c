/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 05:14:37 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/25 14:21:51 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	verif_command_parsing(
	t_command_parsing_result *command_parsing)
{
	if (command_parsing == NULL
		|| !command_parsing->did_succeed)
	{
		ft_lstclear_commands(&command_parsing->command);
		free(command_parsing);
		return (1);
	}
	return (0);
}

int	pipes_management(char **remaining_line)
{
	if (*remaining_line && *remaining_line[0] == '|')
	{
		*remaining_line = skip_one_character(*remaining_line);
		*remaining_line = skip_spaces(*remaining_line);
		if (ft_strlen(*remaining_line) == 0 || *remaining_line[0] == '|')
		{
			ft_putstr_fd(
				"minishell: syntax error near unexpected token `", 2);
			ft_putstr_fd(*remaining_line, 2);
			write (2, "'\n", 2);
			return (1);
		}
	}
	return (0);
}

int	check_remaining_line_length(char *remaining_line,
	t_command_parsing_result *result)
{
	if (ft_strlen(remaining_line) == 0)
	{
		result->did_succeed = TRUE;
		return (1);
	}
	return (0);
}
