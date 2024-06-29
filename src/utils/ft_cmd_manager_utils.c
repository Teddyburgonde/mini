/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmd_manager_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 06:02:12 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/23 08:39:11 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	cleanup_vars_path(t_vars *vars)
{
	if (vars->path)
	{
		ft_free(vars->path);
		vars->path = NULL;
	}
}

void	manage_variable_path(t_env **env, t_vars *vars)
{
	if (lst_search_env("$PATH", *env) != NULL)
		vars->path = ft_split((lst_search_env("$PATH", *env))->value, ':');
}
