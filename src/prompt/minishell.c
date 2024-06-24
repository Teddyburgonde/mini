/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:15:39 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/23 08:40:43 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	g_sig = 0;

int	run_readline(t_env *env, t_vars *vars)
{
	if (ft_readline(&env, vars) == 1)
	{
		ft_lstclear_env(&env);
		free(vars);
		return (1);
	}
	return (0);
}

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
	run_signals();
	if (run_readline(env, vars) == 1)
		return (1);
	ft_lstclear_env(&env);
	free(vars);
	return (0);
}
