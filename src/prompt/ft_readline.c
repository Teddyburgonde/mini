/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 11:20:34 by tebandam          #+#    #+#             */
/*   Updated: 2024/03/24 16:21:23 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_readline(t_env **env, t_parse **parse, t_vars *vars)
{
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
		parsing(vars, parse, env);
		// if (parse->next == NULL)
			// cmd_selector(vars, &env, ac, av);
		// else
			// cmd
		free(vars->input);
	}
	return (0);
}
