/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:36:51 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/29 15:55:09 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static long	ft_atol(const char *nptr)
{
	int			i;
	long long	minus;
	long long	nb;

	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	minus = 1;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			minus *= -1;
		i++;
	}
	nb = 0;
	while (ft_isdigit(nptr[i]) == 1)
	{
		nb = nb * 10 + (nptr[i] - 48);
		if (nb < 0)
			return (0);
		if (nb * minus < LONG_MIN)
			return (0);
		i++;
	}
	return (minus * nb);
}

int	ft_exit(char **command)
{
	unsigned char	tmp;
	int				i;

	i = 0;
	if (command[1] == NULL || command[1][0] == '\0'
		|| ft_strncmp(command[1], "-9223372036854775808", 21) == 0)
		ft_exit_message_0();
	if (ft_isdigit(command[1][i]) == 1)
	{
		while (command[1][i])
		{
			if (ft_isdigit(command[1][i]))
				i++;
			else
				ft_exit_message_2(command[1]);
		}
	}
	if (command[2] != NULL)
		ft_exit_message_too_many_arguments();
	if (ft_atol(command[1]) == 0)
		ft_exit_message_argument_required(command[1]);
	tmp = ft_atol(command[1]) % 256;
	printf("exit\n");
	//exit code 0 cependant la tu peux quitter le programme
	exit(tmp);
}
