/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 14:04:31 by rgobet            #+#    #+#             */
/*   Updated: 2024/03/17 12:09:26 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*get_name(char *line, int j)
{
	char	*tab;
	int		i;

	tab = malloc(sizeof(char) * (j + 1));
	if (!tab)
	{
		write (2, "Initialize environment variable has an error.\n", 46);
		exit(1);
	}
	i = 0;
	while (i != j)
	{
		tab[i] = line[i];
		i++;
	}
	tab[i] = 0;
	return (tab);
}

void	init_env(t_env **env, char **envp)
{
	t_env	*new;
	int		i;
	int		j;

	if (!envp || !envp[0])
		return ;
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] != '=')
			j++;
		new = ft_lstnew_env();
		new->var = copy(&envp[i][j + 1]);
		new->var_name = get_name(envp[i], j);
		new->full_path = copy(envp[i]);
		ft_lstadd_back_env(env, new);
		i++;
	}
}
