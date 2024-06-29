/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_arg_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 04:00:55 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/24 04:04:13 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	verif_arg_env(char **command_line)
{
	if (ft_strcmp(command_line[0], "env") == 0 && command_line[1] != NULL
		&& access(command_line[1], F_OK) == 0)
	{
		ft_putstr_fd("env: ‘", 2);
		ft_putstr_fd(command_line[1], 2);
		ft_putstr_fd("‘: No such file or directory", 2);
		return (1);
	}
	return (0);
}

void	print_env_vars(t_vars *vars, t_redirection *redirect)
{
	int	i;

	i = 0;
	if (vars->nb_cmd == 1)
	{
		while (vars->env[i])
		{
			ft_putstr_fd(vars->env[i], redirect->outfile_fd);
			ft_putstr_fd("\n", redirect->outfile_fd);
			i++;
		}
	}
	else
	{
		while (vars->env[i])
		{
			printf("%s\n", vars->env[i]);
			i++;
		}
	}
}
