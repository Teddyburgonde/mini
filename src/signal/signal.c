/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 00:07:22 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/23 13:29:06 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_ctrl_c(int signal)
{
	extern int	exit_status;

	(void)signal;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	exit_status = 130;
}

void	ctrl_d(int signal)
{
	extern int	exit_status;

	(void)signal;
	ft_putstr_fd("exit\n", 1);
	exit(exit_status);
}

// si variable des signaux et different de la variable global 
// des signaux , on return.
// Variable global dans readline , rl_done qui permet d'arreter
// readline

// peut etre pour utiliser rl_done il faut utiliser une autre variable
// global
