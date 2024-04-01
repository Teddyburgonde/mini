/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:53:00 by rgobet            #+#    #+#             */
/*   Updated: 2024/03/24 16:17:46 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_free(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_readline(t_vars *vars)
{
	printf("exit\n");
	free(vars->input);
	free(vars);
}

int	check_free_readline(t_vars *vars)
{
	if (!vars)
	{
		ft_putstr_fd("Error: malloc is failed !\n", 2);
		return (1);
	}
	return (0);
}
