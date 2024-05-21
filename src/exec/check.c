/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:11:08 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/21 15:22:56 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../minishell.h"

int	check_infile(t_redirection_to_expand *redir)
{
	t_redirection_to_expand	*tmp;

	tmp = redir;
	while (tmp)
	{
		if (tmp->e_type == REDIRECTION_INFILE)
		{
			if (access(tmp->arg, F_OK) != 0 || access(tmp->arg, R_OK) != 0
				|| access(tmp->arg, W_OK) != 0)
			{
				ft_putstr_fd("Error\nPermission denied\n", 2);
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
void	update_full_cmd(char ***full_cmd, char *is_valid_cmd)
{
	free((*full_cmd)[0]);
	(*full_cmd)[0] = NULL;
	(*full_cmd)[0] = malloc(ft_strlen(is_valid_cmd) + 1);
	ft_strlcpy((*full_cmd)[0], is_valid_cmd, ft_strlen(is_valid_cmd) + 1);
	free(is_valid_cmd);
}

void	build_path(char **path, char **bin_path,
	char **is_valid_cmd, char **full_cmd)
{
	int	i;

	i = 0;
	while (path[i])
	{
		*bin_path = ft_strjoin(path[i++], "/");
		*is_valid_cmd = ft_strjoin(*bin_path, full_cmd[0]);
		free(*bin_path);
		if (access(*is_valid_cmd, X_OK) == 0)
		{
			update_full_cmd(&full_cmd, *is_valid_cmd);
			break ;
		}
		free(*is_valid_cmd);
	}
}

char	**find_the_accessible_path(char **path, t_vars *vars, char **command_line)
{
	int		i;
	char	*bin_path;
	char	*is_valid_cmd;

	i = 0;	
	vars->cmd[i] = command_line;
	if (vars->full_cmd == NULL || vars->full_cmd[0] == NULL
		|| vars->full_cmd[0][0] == '\0')
	{
		ft_putstr_fd("Error\nCmd invalid", 2);
		ft_free_tab_3d(vars);
		ft_free(vars->path);
		// ft_free(vars->full_cmd);
		exit(1);
	}
	if (access(vars->full_cmd[0], X_OK) == 0)
		return (vars->full_cmd);
	build_path(path, &bin_path, &is_valid_cmd, vars->full_cmd);
	return (vars->full_cmd);
}

void	verif_tab(char *tab)
{
	if (!tab)
	{
		ft_putstr_fd("Error\n No limiter for heredoc", 2);
		exit(1);
	}
}
