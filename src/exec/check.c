/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 11:11:08 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/18 15:39:41 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../minishell.h"
#include <dirent.h>

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
				ft_putstr_fd("Error\n Permission denied\n", 2);
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

int	build_path(char **path, char **bin_path,
	char **is_valid_cmd, char **full_cmd)
{
	int		i;
	char	*basic_cmd;
	t_bool	successfull;

	i = 0;
	successfull = FALSE;
	basic_cmd = copy(full_cmd[0]);
	while (path && path[i])
	{
		*bin_path = ft_strjoin(path[i++], "/");
		*is_valid_cmd = ft_strjoin(*bin_path, full_cmd[0]);
		free(*bin_path);
		if (access(*is_valid_cmd, X_OK) == 0)
		{
			update_full_cmd(&full_cmd, *is_valid_cmd);
			successfull = TRUE;
			break ;
		}
		free(*is_valid_cmd);
	}
	if (successfull == FALSE)
	{
		if (full_cmd[0][0] == '.' && full_cmd[0][1] == '/')
		{
			DIR *dir = opendir(&full_cmd[0][2]);
			if (access(full_cmd[0], F_OK) != -1)
			{
				if (access(full_cmd[0], X_OK) == -1)
				{
					ft_putstr_fd(" Permission denied\n", 2);
					free(basic_cmd);
					return (126);
				}
			}
			else if (dir)
			{
				ft_putstr_fd(basic_cmd, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				free(basic_cmd);
				closedir(dir);
				return (126);
			}
		}
		else if (full_cmd[0][0] == '.')
			ft_putstr_fd(" Is a directory\n", 2);
		else if (full_cmd[0][0] == '/')
			ft_putstr_fd(" No such file or directory\n", 2);
		else
		{
			ft_putstr_fd(basic_cmd, 2);
			ft_putstr_fd(": command not found\n", 2);
			return (127);
		}
		free(basic_cmd);
		return (127);
	}
	free(basic_cmd);
	return (0);
}

char	**find_the_accessible_path(char **path, t_vars *vars, char **command_line)
{
	int		i;
	char	*bin_path;
	char	*is_valid_cmd;
	DIR		*dir;

	i = 0;	
	if (command_line == NULL || command_line[0] == NULL
		|| command_line[0][0] == '\0')
	{
		ft_putstr_fd(command_line[0], 2);
		ft_putstr_fd(": command not found.\n", 2);
		vars->exit_code = 127;
		return (command_line);
	}
	dir = opendir(&command_line[0][2]);
	if (dir)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command_line[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		closedir(dir);
		vars->exit_code = 126;
		return (command_line);
	}
	if (access(command_line[0], X_OK) == 0)
		return (command_line);
	vars->exit_code = build_path(path, &bin_path, &is_valid_cmd, command_line);
	return (command_line);
}

void	verif_tab(char *tab)
{
	if (!tab)
	{
		ft_putstr_fd("Error\n No limiter for heredoc", 2);
		exit(1);
	}
}
