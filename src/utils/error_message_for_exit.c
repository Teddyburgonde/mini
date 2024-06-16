/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message_for_exit.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 17:39:20 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/16 14:45:34 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_exit_message_0(void)
{
	printf("exit\n");
	//exit code 0 cependant la tu peux quitter le programme
	exit (0);
}

void	ft_exit_message_2(char *command)
{
	ft_putstr_fd("exit\nbash: exit: ", 2);
	ft_putstr_fd(command, 2);
	//ft_putstr_fd(": numeric argument required\n", 2);
}

void	ft_exit_message_too_many_arguments(void)
{
	ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("bash: exit: too many arguments\n", 2);
	//exit code 1 cependant la tu peux quitter le programme
}

void	ft_exit_message_argument_required(char *command)
{
	ft_putstr_fd("exit\nbash: exit: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	exit(2);
}
