/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:58:59 by rgobet            #+#    #+#             */
/*   Updated: 2024/03/17 11:33:04 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_env(t_env *envp)
{
	while (envp)
	{
		printf("Name: %s, var: %s\n", envp->var_name, envp->var);
		envp = envp->next;
	}
}
