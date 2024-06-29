/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif_command_line_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 11:09:52 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/25 13:20:03 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_spaces_and_tabs(char *command_line,
	t_command_line_parsing *parsing_result, t_vars *vars)
{
	if (skip_spaces_and_tabs(command_line) == 0)
	{
		free(parsing_result);
		free(command_line);
		command_line = NULL;
		vars->exit_code = 2;
		return (1);
	}
	return (0);
}

void	add_history_and_parse(char *command_line,
	t_command_line_parsing	**parsing_result)
{
	add_history(command_line);
	*parsing_result = ft_parse_command_line(command_line);
}

int	handle_parsing_errors(t_command_line_parsing	*parsing_result,
	char *command_line, t_vars *vars)
{
	if (!parsing_result->did_succeed)
	{
		free(command_line);
		vars->exit_code = 2;
		ft_lstclear_commands(&parsing_result->commands);
		free(parsing_result);
		return (1);
	}
	return (0);
}

int	check_pipe_position(char *command_line, t_vars *vars)
{
	if (command_line[0] == '|')
	{
		write (2, "Error: too much pipe or in a wrong position !\n", 46);
		vars->exit_code = 2;
		free(command_line);
		return (1);
	}
	return (0);
}

int	check_empty_quotes(char *command_line,
	t_command_line_parsing	*parsing_result, t_vars *vars)
{
	if (ft_strcmp(command_line, "''") == 0
		|| ft_strcmp(command_line, "\"\"") == 0)
	{
		write(2, "Command '' not found, but can be installed with !\n", 50);
		ft_lstclear_commands(&parsing_result->commands);
		vars->exit_code = 2;
		free(parsing_result);
		free(command_line);
		return (1);
	}
	if (parsing_result->commands->arguments != NULL
		&& (ft_strcmp(parsing_result->commands->arguments->content, "''") == 0
			|| ft_strcmp(command_line, "\"\"") == 0)
		&& parsing_result->commands->next == NULL)
	{
		write(2, "Command '' not found, but can be installed with !\n", 50);
		ft_lstclear_commands(&parsing_result->commands);
		free(parsing_result);
		free(command_line);
		vars->exit_code = 2;
		return (1);
	}
	return (0);
}
