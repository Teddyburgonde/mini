/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 07:55:39 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/29 09:05:11 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	update_full_cmd(char ***full_cmd, char *is_valid_cmd)
{
	free((*full_cmd)[0]);
	(*full_cmd)[0] = NULL;
	(*full_cmd)[0] = malloc(ft_strlen(is_valid_cmd) + 1);
	ft_strlcpy((*full_cmd)[0], is_valid_cmd, ft_strlen(is_valid_cmd) + 1);
	free(is_valid_cmd);
}

static t_bool	is_valid_command_in_path(char **path,
	char **full_cmd, char **is_valid_cmd)
{
	char	*bin_path;
	int		i;

	i = 0;
	while (path && path[i])
	{
		bin_path = ft_strjoin(path[i++], "/");
		*is_valid_cmd = ft_strjoin(bin_path, full_cmd[0]);
		free(bin_path);
		if (access(*is_valid_cmd, X_OK) == 0)
		{
			update_full_cmd(&full_cmd, *is_valid_cmd);
			return (TRUE);
		}
		free(*is_valid_cmd);
	}
	return (FALSE);
}

static int	check_and_open_dir(char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir)
	{
		ft_putstr_fd(" Is a directory\n", 2);
		closedir(dir);
		return (126);
	}
	ft_putstr_fd(" No such file or directory\n", 2);
	return (126);
}

static int	handle_path_with_slash(char **full_cmd)
{
	if (ft_strcspn(full_cmd[0], "/") < ft_strlen(full_cmd[0]))
	{
		if (full_cmd[0][0] == '.' && full_cmd[0][1] == '/')
		{
			return (check_and_open_dir(&full_cmd[0][2]));
		}
		else
		{
			ft_putstr_fd(" No such file or directory\n", 2);
			return (127);
		}
	}
	ft_putstr_fd(" command not found\n", 2);
	return (127);
}

int	build_path(char **path, char **bin_path,
	char **is_valid_cmd, char **full_cmd)
{
	char	*basic_cmd;
	t_bool	successfull;
	int		result;

	(void)bin_path;
	successfull = FALSE;
	basic_cmd = copy(full_cmd[0]);
	successfull = is_valid_command_in_path(path, full_cmd, is_valid_cmd);
	if (successfull == FALSE)
	{
		result = handle_path_with_slash(full_cmd);
		free(basic_cmd);
		return (result);
	}
	free(basic_cmd);
	return (0);
}
