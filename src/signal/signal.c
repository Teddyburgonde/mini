/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 00:07:22 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/09 15:11:31 by tebandam         ###   ########.fr       */
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

void	set_interactive_mode(int	set)
{	
	// mode interactif
	if (set == 1)
	{
		// si il y a un SIGINT (ctrl C) on appel ft_ctrl_c 
		signal(SIGINT, &ft_ctrl_c);
		// si il y a un SIGQUIT on l'ignore grace a la macro SIG_IGN
		signal(SIGQUIT, SIG_IGN);
		return ;
	}
	if (set == 2)
	{
		// mode non interactif
		// ignorer les signaux avec SIG_IGN
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		return ;
	}
	if (set == 3)
	{
		// comportement par default de SIGINT ET SINGOUT
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		return ;
	}
	
}

