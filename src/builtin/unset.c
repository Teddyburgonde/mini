/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:45:02 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/17 07:19:33 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_remove_env_variable(t_env **env, char *name)
{
	int	i;
	t_env *tmp;
	t_env *prev;

	i = 1;
	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->var_name, name) == 0)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			free(tmp->var_name);
			free(tmp->full_path);
			free(tmp->value);
			free(tmp);
			return (0);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (1);
}

int	unset(t_env **env, char **names)
{
	int		i;

	i = 1;
	// modif le 17 juin 2024 
	if (!env || !*env)
		return (1);
	while (names[i])
	{
		if (ft_strcmp(names[i], "SHELL") == 0)
			return (1);
		if (ft_remove_env_variable(env, names[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}
