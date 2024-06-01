/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 14:16:48 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/01 10:41:22 by tebandam         ###   ########.fr       */
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

static void	is_option_1(char **command)
{
	int	i;
	int	j;

	i = 2;
	while (command[i])
	{
		j = 0;
		while (command[i][j])
		{
			write (1, &command[i][j], 1);
			j++;
		}
		if (command[i][j] == '\0')
			break ;
		printf(" ");
		i++;
	}
}

static void	is_option_0(char **command)
{
	int	i;

	i = 1;
	while (command[i])
	{
		printf("%s", command[i]);
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
		is_option_1(command);
	if (option == 0)
		is_option_0(command);
}
