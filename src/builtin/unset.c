/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:45:02 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/10 15:10:52 by tebandam         ###   ########.fr       */
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
	}
	return (0);
}
