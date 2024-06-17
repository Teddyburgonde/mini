/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:15:39 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/17 14:48:51 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	g_sig = 0;

int	main(int ac, char **av, char *envp[])
{
	t_env	*env;
	t_vars	*vars;

	vars = malloc(sizeof(t_vars));
	if (!vars)
		return (1);
	env = NULL;
	vars->env = NULL;
	vars->exit_code = 0;
	vars->exit_code_signal = 0;
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
	signal(SIGQUIT, SIG_IGN);
	if (ft_readline(&env, vars) == 1)
	{
		ft_lstclear_env(&env);
		free(vars);
		return (1);
	}
	ft_lstclear_env(&env);
	free(vars);
	return (0);
}

