/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:48:01 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/20 12:58:52 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	fill_command_paths(t_vars *vars, t_argument_to_expand *tmp_arg, t_env *env)
{
	t_argument	*final_parsing;
	char		**command_line;
	int			i;

	i = 0;
	while (i < vars->nb_cmd)
	{
		final_parsing = ft_expand_argument(tmp_arg, env);
		command_line = ft_setup_command(final_parsing);
		vars->full_cmd = command_line;
		vars->cmd[i] = find_the_accessible_path(vars->path, vars, command_line);
		if (vars->cmd[i] == NULL)
		{
			return (-1);
		}
		tmp_arg = tmp_arg->next;
		if (final_parsing)
			ft_lstclear_argument(&final_parsing);
		i++;
	}
	vars->cmd[i] = NULL;
	return (0);
}

int	verif_fill_command_paths(t_vars *vars, t_argument_to_expand *tmp_arg, t_env *env)
{
	if (fill_command_paths(vars, tmp_arg, env) == -1)
	{
		ft_free(vars->path);
		ft_free_tab_3d(vars);
		return (1);
	}
	return (0);
}