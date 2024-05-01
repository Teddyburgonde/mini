/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 04:35:08 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/01 05:20:07 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_env(t_env **env, char *command_line)
{
	t_env	*tmp;

	if (command_line != NULL && access(command_line, F_OK) == 0)
	{
		ft_putstr_fd("env: ‘", 2);
		ft_putstr_fd(&command_line[1], 2);
		ft_putstr_fd("‘: No such file or directory", 2);
	}
	else if (command_line == NULL)
	{
		tmp = *env;
		while (tmp)
		{
			printf("%s\n", tmp->full_path);
			tmp = tmp->next;
		}
	}
}
