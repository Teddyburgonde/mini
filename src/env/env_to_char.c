/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:44:13 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/15 14:44:29 by tebandam         ###   ########.fr       */
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
		tmp[i] = env->full_path;
		env = env->next;
		i++;
	}
	tmp[i] = 0;
	return (tmp);
}
