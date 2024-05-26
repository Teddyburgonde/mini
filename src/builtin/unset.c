/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:45:02 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/26 17:53:55 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	unset(t_env **env, char *name)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->var_name, name) == 0)
		{
			prev->next = tmp->next;
			free(tmp->var_name);
			free(tmp->var);
			free(tmp);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}
