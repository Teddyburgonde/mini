/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 15:10:36 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/01 05:14:12 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// static char	*access_to_cmd(char *cmd, char **path, char *error)
// {
// 	int		i;
// 	char	*tmp;

// 	i = 0;
// 	while (path[i])
// 	{
// 		tmp = ft_strjoin(path[i], cmd[0]);
// 		if (access(tmp, X_OK) == 0)
// 		{
// 			ft_free(path);
// 			free(cmd[0]);
// 			return (tmp);
// 		}
// 		free(tmp);
// 		i++;
// 	}
// 	write(2, "command not found: ", 19);
// 	write(2, error, ft_strlen(error));
// 	write(2, "\n", 1);
// 	return (NULL);
// }

// static char	*verification(t_vars *vars, char **cmd, char **env)
// {
// 	int		i;
// 	char	*tmp;
// 	char	*error;
// 	char	**path;

// 	i = 0;
// 	path = NULL;
// 	while (env[i])
// 	{
// 		if (env[i][0] == 'P' && env[i][1] == 'A' && env[i][2] == 'T'
// 		&& env[i][3] == 'H' && env[i][4] == '=')
// 		{
// 			path = ft_split(&env[i][5], ':');
// 			break ;
// 		}
// 		i++;
// 	}
// 	if (!path)
// 		return (2);
// 	error = copy(cmd[0]);
// 	cmd[0] = slash_add("/", cmd[0]);
// 	tmp = access_to_cmd(cmd[0], path, error);
// 	if (tmp == NULL)
// 		return (NULL);
// 	return (tmp);
// }

// static void	single_cmd(t_vars *vars, char **env)
// {
// 	pid_t	pid;
// 	char	**cmd;

// 	cmd = ft_split(vars->input, ' ');
// 	cmd[0] = verification(vars, cmd, env);
// 	if (cmd[0])
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			ft_free(cmd);
// 			return (1);
// 		}
// 		if (pid == 0)
// 		{
// 			execve(cmd[0], cmd, env);
// 			ft_free(cmd);
// 		}
// 		else
// 		{
// 			waitpid(pid, 0, 2);
// 			ft_free(cmd);
// 			return ;
// 		}
// 	}
// }

static int	cmd_selector(t_env **env, char **command_line)
{
	if (ft_strcmp(command_line[0], "echo") == 0
		|| ft_strcmp(command_line[0], "pwd") == 0
		|| ft_strcmp(command_line[0], "unset") == 0
		|| ft_strcmp(command_line[0], "export") == 0
		|| ft_strcmp(command_line[0], "printenv") == 0
		|| ft_strcmp(command_line[0], "env") == 0
		|| ft_strcmp(command_line[0], "exit") == 0)
	{
		if (ft_strcmp(command_line[0], "echo") == 0)
			ft_echo(command_line);
		else if (ft_strcmp(command_line[0], "pwd") == 0)
			ft_pwd();
		else if (ft_strcmp(command_line[0], "unset") == 0)
			unset(env, command_line[1]);
		else if (ft_strcmp(command_line[0], "export") == 0)
			export(env, command_line);
		else if (ft_strcmp(command_line[0], "printenv") == 0
			|| ft_strcmp(command_line[0], "env") == 0)
			ft_env(env, command_line[1]);
		else if (ft_strcmp(command_line[0], "exit") == 0)
			return (atoi(command_line[1]));
		return (0);
	}
	return (666);
}

static int	ft_command_exec(t_env **env, t_argument *final_parsing,
		t_redirection_to_expand *redirections)
{
	t_redirection_to_expand			*tmp_redir;
	char							**command_line;
	int								exit;

	command_line = ft_setup_command(final_parsing);
	tmp_redir = redirections;
	exit = cmd_selector(env, command_line);
	if (exit == 666)
		write(1, "pipex_bonus\n", 12);
	else if (exit != 0)
	{
		write(2, "Error !\n", 8);
		return (exit);
	}
	ft_free(command_line);
	return (0);
}

int	ft_cmd_manager(t_env **env, t_command_line_parsing_result *cmd)
{
	t_redirection_to_expand			*redirections;
	t_command_to_expand				*tmp;
	t_argument_to_expand			*tmp_arg;
	t_argument						*final_parsing;

	tmp = cmd->commands;
	while (tmp)
	{
		tmp_arg = tmp->arguments;
		redirections = tmp->redirections;
		final_parsing = ft_expand_argument(tmp_arg, *env);
		if (final_parsing != NULL)
			ft_command_exec(env, final_parsing, redirections);
		tmp = tmp->next;
	}
	return (0);
}
