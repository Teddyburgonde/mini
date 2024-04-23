/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 14:16:48 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/23 14:18:40 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_there_an_option_n(char **command)
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

void	is_option_1(char **command)
{
	int	i;
	int	j;

	i = 0;
	j = 2;
	while (command[j])
	{
		while (command[j][i])
		{
			write (2, &command[j][i], 1);
			i++;
		}
		if (command[j + 1])
			write (2, " ", 1);
		i = 0;
		j++;
	}
}

void	is_option_0(char **command)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (command[j])
	{
		while (command[j][i])
		{
			write (2, &command[j][i], 1);
			i++;
		}
		write (2, " ", 1);
		i = 0;
		j++;
	}
	write (2, "\n", 1);
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
