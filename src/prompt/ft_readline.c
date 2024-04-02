/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 11:20:34 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/02 16:45:38 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_readline(t_env **env, t_vars *vars)
{
	t_command_line_parsing_result parsing_result;
	
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
		parsing_result = ft_parse_command_line(command_line);
		if (!parsing_result.did_succeed)
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
