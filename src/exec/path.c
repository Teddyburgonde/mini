/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:48:01 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/24 14:22:34 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

size_t	ft_array_len(char **arr)
{
	size_t	i;

	if (!arr || !arr[0])
		return (0);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

int	verif_fill_command_paths(t_vars *vars, t_command_to_expand *tmp, t_env *env)
{
	if (fill_command_paths(vars, tmp, env) == -1)
	{
		if (vars->path)
		{
			ft_free(vars->path);
			vars->path = NULL;
		}
		ft_free_tab_3d(vars);
		return (1);
	}
	return (0);
}
