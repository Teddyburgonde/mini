/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:36:51 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/26 16:55:50 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <limits.h>

// static int	is_whitespace(char c)
// {
// 	if ((c >= 9 && c <= 13) || c == 32)
// 		return (1);
// 	return (0);
// }

// static unsigned char	ft_char_to_unsigned_char(const char *nptr)
// {
// 	int	i;
// 	int	a;
// 	int	sign;

// 	i = 0;
// 	a = 0;
// 	sign = 1;
// 	while (is_whitespace(nptr[i]))
// 		i++;
// 	while (nptr[i] == '+' || nptr[i] == '-')
// 	{
// 		if (nptr[i] == '-')
// 			sign *= -1;
// 		i++;
// 	}
// 	while (nptr[i] >= '0' && nptr[i] <= '9')
// 	{
// 		a = a * 10 + (nptr[i] - '0');
// 		i++;
// 	}
// 	return (a * sign);
// }

static int	ft_isdigit(int c)
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

static void	ft_exit_message_0(void)
{
	printf("exit\n");
	exit (0);
}

static void	ft_exit_message_2(char *command)
{
	ft_putstr_fd("exit\nbash: exit: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	exit(2);
}

static void	ft_exit_message_too_many_arguments(void)
{
	ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("bash: exit: too many arguments\n", 2);
	exit (1);
}

static void	ft_exit_message_argument_required(char *command)
{
	ft_putstr_fd("exit\nbash: exit: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	exit(2);
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
	exit(tmp);
}
