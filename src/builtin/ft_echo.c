/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 14:16:48 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/02 07:05:36 by tebandam         ###   ########.fr       */
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

void	ft_echo(char **command)
{
	int	option;

	option = is_there_an_option_n(command);
	if (option == 1)
		print_with_option_n(command);
	if (option == 0)
		print_not_option_n(command);
}
