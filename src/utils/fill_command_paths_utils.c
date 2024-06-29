/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_command_paths_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:12:01 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/24 14:24:09 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**copy_double_array(char **src)
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
		while (src[i][j])
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

int	handle_command_line(t_vars *vars,
	char **command_line, t_argument *final_parsing, int i)
{
	if (command_line[0] != NULL)
	{
		vars->full_cmd = find_the_accessible_path(
				vars->path, vars, command_line);
		vars->cmd[i] = copy_double_array(vars->full_cmd);
		if (vars->cmd[i] == NULL)
		{
			ft_lstclear_argument(&final_parsing);
			return (-1);
		}
	}
	else
	{
		vars->cmd[i] = copy_double_array(NULL);
		free(command_line);
	}
	return (0);
}

int	copy_command_line(char **command_line,
	t_vars *vars, int i, t_argument *final_parsing)
{
	vars->cmd[i] = copy_double_array(command_line);
	if (vars->cmd[i] == NULL)
	{
		ft_lstclear_argument(&final_parsing);
		return (-1);
	}
	return (0);
}

int	process_command_path(t_vars *vars,
		t_command_to_expand *tmp, t_env *env, int i)
{
	t_argument	*final_parsing;
	char		**command_line;

	final_parsing = ft_expand_argument(tmp->arguments, env, vars);
	command_line = ft_setup_command(final_parsing);
	if (!is_builtins_parsing(command_line))
	{
		if (handle_command_line(vars, command_line, final_parsing, i) == -1)
			return (-1);
	}
	else
	{
		if (copy_command_line(command_line, vars, i, final_parsing) == -1)
			return (-1);
	}
	if (final_parsing)
		ft_lstclear_argument(&final_parsing);
	return (0);
}

int	fill_command_paths(t_vars *vars,
		t_command_to_expand *tmp, t_env *env)
{
	int	i;

	i = 0;
	while (i < vars->nb_cmd)
	{
		if (process_command_path(vars, tmp, env, i) == -1)
			return (-1);
		i++;
		tmp = tmp->next;
	}
	vars->cmd[i] = NULL;
	return (0);
}
