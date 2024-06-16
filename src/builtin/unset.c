/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:45:02 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/16 11:56:57 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	unset(t_env **env, char **names)
{
	int		i;
	t_env	*tmp;
	t_env	*prev;

	i = 1;
	//exit code 1 si cas d'erreur 
	while (names[i])
	{
		tmp = *env;
		prev = *env;
		while (tmp)
		{
			if (ft_strcmp(tmp->var_name, names[i]) == 0)
			{
				prev->next = tmp->next;
				free(tmp->var_name);
				free(tmp->full_path);
				free(tmp->value);
				free(tmp);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
	return (0);
}
