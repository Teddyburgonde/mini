/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 11:20:34 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/24 11:32:05 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_readline(t_env **env)
{
	t_command_line_parsing_result	*parsing_result;
	t_argument						*final_parsing;
	char							*command_line;

	while (1)
	{
		command_line = readline("minishell > ");
		if (command_line == NULL)
		{
			free(command_line);
			break ;
		}
		add_history(command_line);
		parsing_result = ft_parse_command_line(command_line);
		if (!parsing_result->did_succeed)
		{
			// handle error
		}
		final_parsing = ft_expand_argument(
				parsing_result->commands->arguments, *env);
		// printf("%s\n", parsing_result->commands->arguments->content);
		// if (parse->next == NULL)
			// cmd_selector(vars, &env, ac, av);
		// else
			// cmd
		ft_lstclear_commands(&parsing_result->commands);
		free(parsing_result);
		free(command_line);
	}
	return (0);
}
