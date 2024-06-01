/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:03:38 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/01 16:09:06 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <unistd.h>

static void	ft_close_fd(t_vars *vars)
{
	if (vars->pipe_1[0] != -1)
	{
		close(vars->pipe_1[0]);
		vars->pipe_1[0] = -1;
	}
	if (vars->pipe_1[1] != -1)
	{
		close(vars->pipe_1[1]);
		vars->pipe_1[1] = -1;
	}
	if (vars->pipe_2[0] != -1)
	{
		close(vars->pipe_2[0]);
		vars->pipe_2[0] = -1;
	}
	if (vars->pipe_2[1] != -1)
	{
		close(vars->pipe_2[1]);
		vars->pipe_2[1] = -1;
	}
}

/*
* Utilisation de deux pipes.
* Gerer les redirections comme sur ardoise sauf si les fd infile ou outfile sont > 2
* car 0 et 1 sont les redirections par default.
* Donc close si > 2.
* Systeme de pair et impair pour gestion des pipes.
* set follow-fork-mode child
*/

static void	ft_flow_redirection(t_vars *vars, t_redirection *redirect)
{
	if (vars->cmd_index == 1 && redirect->next != NULL)
		first_command_redirection(vars, redirect);
	else if (vars->cmd_index != 1 && redirect->next != NULL)
	{
		if (vars->cmd_index % 2 == 1)
			pipe_command_redirection_odd(redirect, vars);
		else
			pipe_command_redirection_even(redirect, vars);
	}
	else if (redirect->next == NULL)
	{
		if (vars->cmd_index % 2 == 1)
			last_command_redirection_odd(redirect, vars);
		else
			last_command_redirection_even(redirect, vars);
	}
}

static int	child_process(t_vars *vars, t_redirection *redirect
		, char **actual_cmd)
{
	ft_flow_redirection(vars, redirect);
	if (redirect->infile_fd == -1 || redirect->outfile_fd == -1)
	{
		perror("Error opening files");
		exit(1);
	}
	ft_close_fd(vars);
	execve(actual_cmd[0], actual_cmd, vars->env);
	ft_close_fd(vars);
	if (redirect->infile_fd != -1)
	{
		close(redirect->infile_fd);
		redirect->infile_fd = -1;
	}
	if (redirect->outfile_fd != -1)
	{
		close(redirect->outfile_fd);
		redirect->outfile_fd = -1;
	}
	perror("Execve");
	return (1);
}

/*
* Si heredoc/infile/outfile/append c'est la sortie/entree.
* Si pas de redirection envoie dans le pipe puis dans le parent STDOUT ou renvoie
* dans le pipe si redirect->e_type == PIPE_OUT
*/

static	int	parent_process(t_vars *vars, t_redirection *redirect)
{
	vars->child = fork();
	if (vars->child == 0)
		child_process(vars, redirect, vars->cmd[vars->cmd_index - 1]);
	else if (vars->child < 0)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	if ((vars->cmd_index - 1) % 2 == 1 && vars->nb_cmd > 1)
	{
		if (vars->pipe_1[1] != -1)
		{
			close(vars->pipe_1[1]);
			vars->pipe_1[1] = -1;
		}
		if (vars->pipe_2[0] != -1)
		{
			close(vars->pipe_2[0]);
			vars->pipe_2[0] = -1;
		}
	}
	else if ((vars->cmd_index - 1) % 2 == 0 && vars->nb_cmd > 1)
	{
		if (vars->pipe_1[0] != -1)
		{
			close(vars->pipe_1[0]);
			vars->pipe_1[0] = -1;
		}
		if (vars->pipe_2[1] != -1)
		{
			close(vars->pipe_2[1]);
			vars->pipe_2[1] = -1;
		}
	}
	if (vars->cmd_index == vars->nb_cmd)
		vars->last_child = vars->child;
	return (0);
}


// t_bool	is_builtins_parsing(char **str)
// {
// 	if (ft_strcmp(str[0], "unset") == 0
// 		|| ft_strcmp(str[0], "export") == 0
// 		|| ft_strcmp(str[0], "cd") == 0
// 		|| ft_strcmp(str[0], "pwd") == 0
// 		|| ft_strcmp(str[0], "echo") == 0
// 		|| ft_strcmp(str[0], "exit") == 0
// 		|| ft_strcmp(str[0], "env") == 0)
// 		return (TRUE);
// 	return (FALSE);
// }

// t_bool	is_builtins_exec(t_vars *vars)
// {
// 	if (ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "unset") == 0
// 		|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "export") == 0
// 		|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "cd") == 0
// 		|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "pwd") == 0
// 		|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "echo") == 0
// 		|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "exit") == 0
// 		|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "env") == 0)
// 		return (TRUE);
// 	return (FALSE);
// }

/*
* Wait the last process will getting the exit code of the last process.
*/

static int	wait_process(t_vars *vars)
{
	int		wstatus;
	int		exit_status;
	pid_t	pid;

	wstatus = 0;
	pid = 0;
	while (pid != -1)
	{
		pid = wait(&wstatus);
		if (pid == vars->last_child)
			exit_status = WEXITSTATUS(wstatus);
	}
	return (exit_status);
}

int	fork_processes(t_vars *vars, t_redirection **redirect, t_env **envp)
{
	t_redirection	*tmp;

	tmp = *redirect;
	vars->cmd_index = 1;
	vars->pipe_1[0] = -1;
	vars->pipe_1[1] = -1;
	vars->pipe_2[0] = -1;
	vars->pipe_2[1] = -1;
	vars->last_child = -2;
	while (vars->cmd_index <= vars->nb_cmd)
	{
		if (!is_builtins_exec(vars))
		{
			if ((vars->cmd_index - 1) % 2 == 1)
			{
				if (pipe(vars->pipe_1) == -1)
				{
					close(vars->pipe_2[0]);
					close(vars->pipe_2[1]);
					return (EXIT_FAILURE);
				}
			}
			if ((vars->cmd_index - 1) % 2 == 0)
			{
				if (pipe(vars->pipe_2) == -1)
				{
					close(vars->pipe_1[0]);
					close(vars->pipe_1[1]);
					return (EXIT_FAILURE);
				}
			}
			parent_process(vars, tmp);
		}
		else
		{
			// Tri ordre décroissant pour env
			// ft_flow_redirection(vars, tmp);
			cmd_selector(envp, vars->cmd[vars->cmd_index - 1]);
			// close(tmp->outfile_fd);
			// tmp->outfile_fd = -1;
			if (ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "unset") == 0
				|| ft_strcmp(vars->cmd[vars->cmd_index - 1][0], "export") == 0)
			{
				if (vars->env)
					free(vars->env);
				// Impossible de export, le lst_addback ne fonctionne pas ????
				vars->env = env_to_char(*envp);
			}
		}
		tmp = tmp->next;
		vars->cmd_index++;
	}
	wait_process(vars);
	ft_close_fd(vars);
	ft_lstclear_final_redirection(redirect);
	ft_free_tab_3d(vars);
	return (0);
}
