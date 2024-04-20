/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:53:00 by rgobet            #+#    #+#             */
/*   Updated: 2024/04/20 14:16:46 by tebandam         ###   ########.fr       */
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
// void	ft_free_vars_input(t_vars *vars, char **env)
// {
// 	ft_lstclear_env(&env);
// 	free(vars->input);
// 	free(vars);
// 	exit(0);
// }