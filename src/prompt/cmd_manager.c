/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 15:10:36 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/03 11:32:34 by tebandam         ###   ########.fr       */
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
	t_env	*tmp;

	tmp = *env;
	if (ft_strcmp(command_line[0], "echo") == 0)
	{
		ft_echo(command_line);
		return (0);
	}
	else if (ft_strcmp(command_line[0], "pwd") == 0)
	{
		ft_pwd();
		return (0);
	}
	else if (ft_strcmp(command_line[0], "unset") == 0)
	{
		unset(env, command_line[1]);
		return (0);
	}
	else if (ft_strcmp(command_line[0], "export") == 0)
	{
		export(env, command_line);
		return (0);
	}
	else if (ft_strcmp(command_line[0], "printenv") == 0
		|| ft_strcmp(command_line[0], "env") == 0)
	{
		if (ft_strcmp(command_line[0], "env") == 0 && command_line[1] != NULL
			&& access(command_line[1], F_OK) == 0)
		{
			ft_putstr_fd("env: ‘", 2);
			ft_putstr_fd(command_line[1], 2);
			ft_putstr_fd("‘: No such file or directory", 2);
		}
		else if (ft_strcmp(command_line[0], "env") == 0
			&& command_line[1] == NULL)
		{
			while (tmp)
			{
				printf("%s\n", tmp->full_path);
				tmp = tmp->next;
			}
		}
		return (0);
	}
	else if (ft_strcmp(command_line[0], "exit") == 0)
		return (atoi(command_line[1]));
	return (666);
}

int	ft_cmd_manager(t_env **env, t_command_line_parsing_result *cmd)
{
	t_command_to_expand				*tmp;
	t_redirection_to_expand			*tmp_redir;
	t_argument_to_expand			*tmp_arg;
	t_argument						*final_parsing;
	char							**command_line;
	int								exit;

	tmp = cmd->commands;
	while (tmp)
	{
		tmp_arg = tmp->arguments;
		final_parsing = ft_expand_argument(tmp_arg, *env);
		if (final_parsing != NULL)
		{
			command_line = ft_setup_command(final_parsing);
			tmp_redir = tmp->redirections;
			exit = cmd_selector(env, command_line);
			if (exit == 666)
				write(1, "pipex_bonus\n", 12);
			else if (exit != 0)
				write(2, "Error !\n", 8);
		}
		if (final_parsing)
			ft_lstclear_argument(&final_parsing);
		ft_free(command_line);
		tmp = tmp->next;
	}
	return (0);
}