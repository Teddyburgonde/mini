/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:44:13 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/23 03:54:33 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**env_to_char(t_env *env)
{
	char	**tmp;
	int		len;
	int		i;

	len = ft_lstsize_env(env);
	len -= count_hidden_var(env);
	tmp = malloc((len + 1) * sizeof(char *));
	if (!tmp)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->hide == FALSE)
			tmp[i] = env->full_path;
		else
			i--;
		env = env->next;
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}
