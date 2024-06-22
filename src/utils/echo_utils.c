/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:33:20 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/20 16:36:10 by tebandam         ###   ########.fr       */
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

void	print_with_option_n(char **command)
{
	int	i;

	i = 2;
	while (command[i])
	{
		printf("%s", command[i]);
		i++;
	}
}

void	print_not_option_n(char **command)
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

void	print_with_option_n_fd(char **command, int fd)
{
	int	i;

	i = 2;
	while (command[i])
	{
		ft_putstr_fd(command[i], fd);
		i++;
	}
}

void	print_not_option_n_fd(char **command, int fd)
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
