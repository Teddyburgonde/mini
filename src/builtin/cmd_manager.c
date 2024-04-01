/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 15:10:36 by rgobet            #+#    #+#             */
/*   Updated: 2024/03/17 16:13:09 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*access_to_cmd(char *cmd, char **path, char *error)
{
	int		i;
	char	*tmp;

	i = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], cmd[0]);
		if (access(tmp, X_OK) == 0)
		{
			ft_free(path);
			free(cmd[0]);
			return (tmp);
		}
		free(tmp);
		i++;
	}
	write(2, "command not found: ", 19);
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
	return (NULL);
}

static char	*verification(t_vars *vars, char **cmd, char **env)
{
	int		i;
	char	*tmp;
	char	*error;
	char	**path;

	i = 0;
	path = NULL;
	while (env[i])
	{
		if (env[i][0] == 'P' && env[i][1] == 'A' && env[i][2] == 'T'
		&& env[i][3] == 'H' && env[i][4] == '=')
		{
			path = ft_split(&env[i][5], ':');
			break ;
		}
		i++;
	}
	if (!path)
		return (2);
	error = copy(cmd[0]);
	cmd[0] = slash_add("/", cmd[0]);
	tmp = access_to_cmd(cmd[0], path, error);
	if (tmp == NULL)
		return (NULL);
	return (tmp);
}

static void	single_cmd(t_vars *vars, char **env)
{
	pid_t	pid;
	char	**cmd;

	cmd = ft_split(vars->input, ' ');
	cmd[0] = verification(vars, cmd, env);
	if (cmd[0])
	{
		pid = fork();
		if (pid == -1)
		{
			ft_free(cmd);
			exit(1);
		}
		if (pid == 0)
		{
			execve(cmd[0], cmd, env);
			ft_free(cmd);
		}
		else
		{
			waitpid(pid, 0, 2);
			ft_free(cmd);
			return ;
		}
	}
}

void	cmd_selector(t_vars *vars, char **env, int ac, char **av)
{
	t_env	*tmp;

	tmp = env;
	if (ft_strcmp(vars->input, "echo") == 0)
		ft_echo(ac, av);
	else if (ft_strcmp(vars->input, "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(vars->input, "unset") == 0)
		unset(&env, av[1]);
	else if (ft_strcmp(vars->input, "export") == 0)
		export(&env, av);
	else if (ft_strcmp(vars->input, "printenv") == 0
		|| ft_strcmp(vars->input, "env") == 0)
	{
		while (tmp)
		{
			printf("%s\n", tmp->full_path);
			tmp = tmp->next;
		}
	}
	else if (vars->input[0] == 'q' && vars->input[1] == 0)
	{
		ft_lstclear(&env);
		free(vars->input);
		free(vars);
		exit(0);
	}
}
