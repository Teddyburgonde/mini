/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:15:39 by tebandam          #+#    #+#             */
/*   Updated: 2024/03/24 16:14:37 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	main(int ac, char **av, char *envp[])
{
	t_env	*env;
	t_parse	*parse;
	t_vars	*vars;

	env = NULL;
	parse = NULL;
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
	if (ft_readline(&env, &parse, vars) == 1)
	{
		ft_lstclear_env(&env);
		ft_lstclear_parse(&parse);
		return (1);
	}
	ft_lstclear_parse(&parse);
	ft_lstclear_env(&env);
	return (0);
}
