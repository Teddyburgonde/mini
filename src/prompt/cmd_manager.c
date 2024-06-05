/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 15:10:36 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/05 14:28:40 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Met des " " après premier '='
// Affiche toutes les vars
// Cache les vars avec value=NULL dans cmd env

static void	exec_env(char **command_line, t_vars *vars)
{
	int	i;

	i = 0;
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
		while (vars->env[i])
		{
			printf("%s\n", vars->env[i]);
			i++;
		}
	}
}

static void	print_env(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		printf("declare -x %s\n", env[i]);
		i++;
	}
}

int	cmd_selector(t_env **env, char **command_line, t_vars *vars)
{
	t_env	*tmp;
	char	**envp;

	tmp = *env;
	envp = NULL;
	if (ft_strcmp(command_line[0], "echo") == 0)
		ft_echo(command_line);
	else if (ft_strcmp(command_line[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(command_line[0], "unset") == 0)
		unset(env, command_line[1]);
	else if (ft_strcmp(command_line[0], "export") == 0)
	{
		if (command_line[1])
			export(env, command_line);
		else
		{
			envp = env_to_char_export(tmp);
			print_env(envp);
			if (envp)
				ft_free(envp);
		}
	}
	else if (ft_strcmp(command_line[0], "cd") == 0)
		ft_cd(command_line, env);
	else if (ft_strcmp(command_line[0], "printenv") == 0
		|| ft_strcmp(command_line[0], "env") == 0)
		exec_env(command_line, vars);
	else if (ft_strcmp(command_line[0], "exit") == 0)
		ft_exit(command_line);
	else
		return (1);
	return (0);
}

int	ft_cmd_manager(t_env **env, t_command_line_parsing_result *cmd)
{
	t_vars							vars;
	t_command_to_expand				*tmp;
	t_redirection_to_expand			*tmp_redir;
	t_argument_to_expand			*tmp_arg;
	t_redirection					*redirection;

	tmp = cmd->commands;
	tmp_redir = tmp->redirections;
	tmp_arg = tmp->arguments;
	vars.env = NULL;
	if (env)
	{
		tmp->redirections = ft_expand_redirections(tmp_redir, *env);
		redirection = stock_redirection(tmp);
		vars.nb_cmd = ft_lstsize_command(cmd->commands);
		vars.path = ft_split(lst_search_env("$PATH", *env)->value, ':');
		vars.cmd = ft_calloc(vars.nb_cmd + 1, sizeof(char **));
		if (redirection->e_position != HERE && tmp_arg != NULL)
			verif_fill_command_paths(&vars, tmp, *env);
		ft_free(vars.path);
		vars.env = env_to_char(*env);
		fork_processes(&vars, &redirection, env);
		free(vars.env);
	}
	return (0);
}
