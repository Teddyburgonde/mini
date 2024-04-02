/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:15:39 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/02 16:46:58 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	main(int ac, char **av, char *envp[])
{
	t_env	*env;
	t_vars	*vars;

	env = NULL;
	vars = NULL;
	(void)av;
	if (ac != 1)
	{
		ft_putstr_fd("Error: Too much arguments\n", 2);
		return (1);
	}
	init_env(&env, envp);
	if (env == NULL)
		return (1);
	signal(SIGINT, ft_ctrl_c);
	signal(SIGQUIT, (__sighandler_t)1);
	if (ft_readline(&env, vars) == 1)
	{
		ft_lstclear_env(&env);
		return (1);
	}
	ft_lstclear_env(&env);
	return (0);
}
