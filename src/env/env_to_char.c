/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:44:13 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/28 16:00:06 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**env_to_char(t_env *env)
{
	char	**tmp;
	int		i;

	tmp = malloc((ft_lstsize_env(env) + 1) * sizeof(char *));
	if (!tmp)
		return (NULL);
	i = 0;
	while (env)
	{
		// tmp[i] = copy(env->full_path);
		tmp[i] = env->full_path;
		env = env->next;
		i++;
	}
	tmp[i] = 0;
	return (tmp);
}
