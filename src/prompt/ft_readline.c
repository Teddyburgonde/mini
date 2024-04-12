/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 11:20:34 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/12 15:50:45 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_readline(t_env **env, t_vars *vars)
{
	t_command_line_parsing_result *parsing_result;

	(void)env;
	vars = malloc(sizeof(t_vars));
	if (check_free_readline(vars) == 1)
		return (1);
	while (1)
	{
		vars->input = readline("minishell > ");
		if (vars->input == NULL)
		{
			free_readline(vars);
			break ;
		}
		add_history(vars->input);
		parsing_result = ft_parse_command_line(vars->input);
		if (!parsing_result->did_succeed)
		{
			// handle error
		}
		// if (parse->next == NULL)
			// cmd_selector(vars, &env, ac, av);
		// else
			// cmd
		free(vars->input);
	}
	return (0);
}
