/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:48:01 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/21 15:48:01 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	fill_command_paths(t_vars *vars, t_command_to_expand *tmp, t_env *env)
{
	t_argument				*final_parsing;
	char					**command_line;
	int						i;

	i = 0;
	while (i < vars->nb_cmd)
	{
		final_parsing = ft_expand_argument(tmp->arguments, env);
		command_line = ft_setup_command(final_parsing);
		vars->full_cmd = command_line;
		vars->cmd[i] = find_the_accessible_path(vars->path, vars, command_line);
		if (vars->cmd[i] == NULL)
			return (-1);
		i++;
		tmp = tmp->next;
	}
	// if (final_parsing)
	// 	ft_lstclear_argument(&final_parsing);
	vars->cmd[i] = NULL;
	return (0);
}

int	verif_fill_command_paths(t_vars *vars, t_command_to_expand *tmp, t_env *env)
{
	if (fill_command_paths(vars, tmp, env) == -1)
	{
		ft_free(vars->path);
		ft_free_tab_3d(vars);
		return (1);
	}
	return (0);
}