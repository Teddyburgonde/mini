/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:58:59 by rgobet            #+#    #+#             */
/*   Updated: 2024/03/08 20:37:45 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_env *envp)
{
	while (envp)
	{
		printf("Name: %s, var: %s\n", envp->var_name, envp->var);
		envp = envp->next;
	}
}