/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   len_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:33:48 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/20 10:35:39 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_lstsize_command(t_command_to_expand *cmd)
{
	int	counter;

	counter = 0;
	while (cmd != NULL)
	{
		cmd = cmd->next;
		counter++;
	}
	return (counter);
}

int	ft_lstsize_env(t_env *env)
{
	int	counter;

	counter = 0;
	while (env != NULL)
	{
		env = env->next;
		counter++;
	}
	return (counter);
}
