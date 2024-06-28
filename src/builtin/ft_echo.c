/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 14:16:48 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/28 19:48:43 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	print_value_exit_status(char **command)
{
	if (command[1][0] && command[1][0] == '$' && command[1][1] == '?')
		return (0);
	return (1);
}

static void	run(t_vars *vars, int option,
	char **command, t_redirection *redirect)
{
	if (vars->nb_cmd == 1)
	{
		if (option == 1)
			print_with_option_n_fd(command, redirect->outfile_fd);
		if (option == 0)
			print_not_option_n_fd(command, redirect->outfile_fd);
	}
	else
	{
		if (option == 1)
			print_with_option_n(command);
		if (option == 0)
			print_not_option_n(command);
	}
}

int	ft_echo(char **command, t_vars *vars, t_redirection *redirect)
{
	int	option;

	if (!command[1])
	{
		write (1, "\n", 1);
		return (0);
	}
	if (print_value_exit_status(command) == 0)
	{
		printf("%d\n", vars->exit_code);
		return (0);
	}
	option = is_there_an_option_n(command);
	run(vars, option, command, redirect);
	return (0);
}
