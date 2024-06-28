/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:36:51 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/28 21:43:37 by tebandam         ###   ########.fr       */
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

int	has_invalid_argument(char *arg, char *next_arg)
{
	int	i;

	i = 0;
	if (ft_isdigit(arg[i]) == 1 && next_arg != NULL)
	{
		ft_putstr_fd(" too many arguments\n", 2);
		return (1);
	}
	while (arg[i])
	{
		if (arg[i] == '-')
			exit (156);
		if (arg[i] == '+')
			exit (100);
		if (!ft_isdigit(arg[i]))
		{
			ft_putstr_fd(" numeric argument required\n", 2);
			exit (2);
		}
		i++;
	}
	if (ft_atol(arg) == 0)
		ft_exit_message_argument_required(arg);
	return (0);
}

int	ft_exit(char **command)
{
	unsigned char	tmp;
	int				i;

	i = 0;
	if (command[1] == NULL || command[1][0] == '\0'
		|| ft_strncmp(command[1], "-9223372036854775808", 21) == 0)
		ft_exit_message_0();
	if (has_invalid_argument(command[1], command[2]))
		exit(1);
	tmp = ft_atol(command[1]) % 256;
	printf("exit\n");
	exit(tmp);
	return (0);
}
