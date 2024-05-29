/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 14:04:31 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/29 15:24:11 by rgobet           ###   ########.fr       */
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
		return (NULL);
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

char	*update_shlvl(int shlvl)
{
	char	*result;
	int		tmp;
	int		len;

	if (shlvl > 255 || shlvl <= 0)
		shlvl = 1;
	len = 0;
	tmp = shlvl;
	while (tmp / 10 != 0)
	{
		tmp /= 10;
		len++;
	}
	if (shlvl > 9 && (shlvl % 10 == 0 || tmp != 0))
		len++;
	if (shlvl % 10 != 0 && shlvl < 10)
		len++;
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	result[len] = 0;
	while (len != 0)
	{
		if (len > 1)
			result[len - 1] = shlvl % 10 + '0';
		else
			result[len - 1] = shlvl + '0';
		shlvl /= 10;
		len--;
	}
	return (result);
}

// Faire un char ** et sort dessus
// Pour env il faut print t_env

void	init_env(t_env **env, char **envp)
{
	t_env	*new;
	char	*tmp;
	int		i;
	int		j;

	if (!envp || !envp[0])
	{
		*env = NULL;
		return ;
	}
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] != '=')
			j++;
		new = ft_lstnew_env();
		new->var_name = get_name(envp[i], j);
		if (ft_strcmp(new->var_name, "SHLVL") == 0)
			new->value = update_shlvl(atoi(&envp[i][j + 1]) + 1);
		else
			new->value = copy(&envp[i][j + 1]);
		if (ft_strcmp(new->var_name, "SHLVL") == 0)
		{
			tmp = ft_strjoin(new->var_name, "=");
			new->full_path = ft_strjoin_mod(tmp, new->value);
		}
		else
			new->full_path = copy(envp[i]);
		ft_lstadd_back_env(env, new);
		i++;
	}
}
