/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:24:05 by rgobet            #+#    #+#             */
/*   Updated: 2024/03/03 16:36:51 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export(t_env **env, char **cmd)
{
	t_env	*tmp_env;
	t_env	*tmp;

	tmp = ft_lstnew();
	tmp_env = lst_search_env(cmd[1], *env)
	if (!cmd[1])
		print_env(*env);
	if (!tmp_env)
	{
		tmp->var_name = cmd[1];
		tmp->var = cmd[2];
		tmp->next = NULL;
		ft_lstadd_back(env, tmp);
	}
	else
	{
		tmp_env = *env;
		while (!ft_strcmp(tmp_env->var_name, cmd[1]))
			tmp_env = tmp_env->next;
		free(tmp_env->var);
		tmp_env->var = cmd[2];
	}
}
