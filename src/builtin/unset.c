/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:45:02 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/28 16:05:57 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	unset(t_env **env, char *name)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = *env;
	//exit code 1 si cas d'erreur 
	while (tmp)
	{
		if (ft_strcmp(tmp->var_name, name) == 0)
		{
			prev->next = tmp->next;
			free(tmp->var_name);
			free(tmp->value);
			free(tmp);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}
