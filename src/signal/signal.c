/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 00:07:22 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/20 19:02:39 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_ctrl_c(int signal)
{
	g_sig = signal;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// void	ctrl_d(int signal)
// {

// 	g_sig = signal;
// 	ft_putstr_fd("exit\n", 1);
// 	exit(g_sig);
// }

void	set_interactive_mode(int set)
{
	if (set == 1)
	{
		signal(SIGINT, &ft_ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		return ;
	}
	if (set == 2)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		return ;
	}
	if (set == 3)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		return ;
	}
}

void	run_signals(void)
{
	signal(SIGINT, ft_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}

