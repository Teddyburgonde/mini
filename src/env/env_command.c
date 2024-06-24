/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 13:01:19 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/24 04:03:20 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	exec_env(char **command_line, t_vars *vars, t_redirection *redirect)
{
	int	i;

	i = 0;
	if (verif_arg_env(command_line) == 1)
		return ;
	if (ft_strcmp(command_line[0], "env") == 0 && command_line[1] == NULL)
		print_env_vars(vars, redirect);
}

void	print_env(char **env, t_vars *vars, t_redirection *redirect)
{
	int	i;

	i = 0;
	if (vars->nb_cmd == 1)
	{
		while (env && env[i])
		{
			ft_putstr_fd("declare -x ", redirect->outfile_fd);
			ft_putstr_fd(env[i], redirect->outfile_fd);
			ft_putstr_fd("\n", redirect->outfile_fd);
			i++;
		}
	}
	else
	{
		while (env && env[i])
		{
			printf("declare -x %s\n", env[i]);
			i++;
		}
	}
}
