/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 21:30:05 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/01 21:30:30 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_close_fd(t_vars *vars)
{
	if (vars->pipe_1[0] != -1)
	{
		close(vars->pipe_1[0]);
		vars->pipe_1[0] = -1;
	}
	if (vars->pipe_1[1] != -1)
	{
		close(vars->pipe_1[1]);
		vars->pipe_1[1] = -1;
	}
	if (vars->pipe_2[0] != -1)
	{
		close(vars->pipe_2[0]);
		vars->pipe_2[0] = -1;
	}
	if (vars->pipe_2[1] != -1)
	{
		close(vars->pipe_2[1]);
		vars->pipe_2[1] = -1;
	}
}
