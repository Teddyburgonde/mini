/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:48:01 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/24 15:49:52 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static size_t	ft_array_len(char **arr)
{
	size_t	i;

	if (!arr || !arr[0])
		return (0);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

static char	**copy_double_array(char **src)
{
	char	**dest;
	char	*tmp;
	size_t	i;
	size_t	j;

	i = 0;
	dest = malloc((ft_array_len(src) + 1) * sizeof(char *));
	if (!dest)
		return (NULL);
	while (src && src[i])
	{
		j = 0;
		tmp = malloc((ft_strlen(src[i]) + 1) * sizeof(char));
		while(src[i][j])
		{
			tmp[j] = src[i][j];
			j++;
		}
		tmp[j] = 0;
		dest[i] = tmp;
		i++;
	}
	dest[i] = NULL;
	ft_free(src);
	return (dest);
}


static int	fill_command_paths(t_vars *vars, t_command_to_expand *tmp, t_env *env)
{
	t_argument				*final_parsing;
	char					**command_line;
	int						i;

	i = 0;
	while (i < vars->nb_cmd)
	{
		final_parsing = ft_expand_argument(tmp->arguments, env);
		command_line = ft_setup_command(final_parsing);

/* 		vars->full_cmd = command_line;
		vars->cmd[i] = find_the_accessible_path(vars->path, vars, command_line);
		vars->cmd[i] = copy_double_array(vars->cmd[i]); */

		vars->full_cmd = find_the_accessible_path(vars->path, vars, command_line);
		vars->cmd[i] = copy_double_array(vars->full_cmd);
		if (vars->cmd[i] == NULL)
			return (-1);
		i++;
		tmp = tmp->next;
		if (final_parsing)
			ft_lstclear_argument(&final_parsing);
	}
	vars->cmd[i] = NULL;
	return (0);
}

int	verif_fill_command_paths(t_vars *vars, t_command_to_expand *tmp, t_env *env)
{
	if (fill_command_paths(vars, tmp, env) == -1)
	{
		ft_free(vars->path);
		ft_free_tab_3d(vars);
		return (1);
	}
	return (0);
}