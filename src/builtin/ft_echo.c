/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 14:16:48 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/07 13:20:08 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_there_an_option_n(char **command)
{
	int	i;
	int	option;

	i = 2;
	option = 0;
	if (command[1][0] == '-' && command[1][1] == 'n')
	{
		while (command[1][i])
		{
			if (command[1][i] != 'n')
				return (option);
			i++;
		}
		if (command[1][i] == '\0')
			option = 1;
	}
	return (option);
}

static void	print_with_option_n(char **command)
{
	int	i;

	i = 2;
	while (command[i])
	{
		printf("%s", command[i]);
		i++;
	}
}

static void	print_not_option_n(char **command)
{
	int	i;

	i = 1;
	while (command[i])
	{
		printf("%s", command[i]);
		if (command[i + 1] != NULL)
			printf(" ");
		i++;
	}
	printf("\n");
}

static void	print_with_option_n_fd(char **command, int fd)
{
	int	i;

	i = 2;
	while (command[i])
	{
		ft_putstr_fd(command[i], fd);
		i++;
	}
}

static void	print_not_option_n_fd(char **command, int fd)
{
	int	i;

	i = 1;
	while (command[i])
	{
		ft_putstr_fd(command[i], fd);
		if (command[i + 1] != NULL)
			ft_putstr_fd(" ", fd);
		i++;
	}
	ft_putstr_fd("\n", fd);
}

void	ft_echo(char **command, t_vars *vars, t_redirection *redirect)
{
	int	option;

	option = is_there_an_option_n(command);
	// if (vars->nb_cmd == 1)
	// {
		if (option == 1)
			print_with_option_n_fd(command, redirect->outfile_fd);
		if (option == 0)
			print_not_option_n_fd(command, redirect->outfile_fd);
	// }
	// else
	// {
	// 	if (option == 1)
	// 		print_with_option_n(command);
	// 	if (option == 0)
	// 		print_not_option_n(command);
	// }
}
