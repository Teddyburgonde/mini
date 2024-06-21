/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:45:02 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/20 18:52:21 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_env_variable(t_env *env_var)
{
	if (env_var)
	{
		free(env_var->var_name);
		free(env_var->full_path);
		free(env_var->value);
		free(env_var);
	}
}

static int	ft_remove_env_variable(t_env **env, char *name)
{
	t_env	*tmp;
	t_env	*prev;

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
			free_env_variable(tmp);
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
	if (!env || !*env)
		return (1);
	while (names[i])
	{
		ft_remove_env_variable(env, names[i]);
		i++;
	}
	return (0);
}
