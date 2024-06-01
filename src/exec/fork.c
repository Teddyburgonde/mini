/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:03:38 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/01 20:14:03 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <readline/readline.h>
#include <stdlib.h>
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

static int    child_process(t_vars *vars, t_redirection *redirect
        , char **actual_cmd, t_env **envp)
{
    ft_flow_redirection(vars, redirect);
    if (redirect->infile_fd == -1 || redirect->outfile_fd == -1)
    {
        perror("Error opening files");
        exit(1);
    }
    ft_close_fd(vars);
    if (is_builtins_exec(vars) == 1) {
        cmd_selector(envp, vars->cmd[vars->cmd_index - 1]);
        exit(0);
    }
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

static	int	parent_process(t_vars *vars, t_redirection *redirect, t_env **envp)
{
	vars->child = fork();
	if (vars->child == 0)
		child_process(vars, redirect, vars->cmd[vars->cmd_index - 1], envp);
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

void	initialize_vars(t_vars *vars)
{
	vars->cmd_index = 1;
    vars->pipe_1[0] = -1;
    vars->pipe_1[1] = -1;
    vars->pipe_2[0] = -1;
    vars->pipe_2[1] = -1;
    vars->last_child = -2;
}

int	setup_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	return (0);
}

int	choice_pipe_setup(t_vars *vars)
{
	if ((vars->cmd_index - 1) % 2 == 1)
		return (setup_pipe(vars->pipe_1));
	else
	 	return (setup_pipe(vars->pipe_2));
}

void	process_commands(t_vars *vars, t_redirection **redirect, t_env **envp)
{
	t_redirection *tmp;

	tmp = *redirect;
    while (vars->cmd_index <= vars->nb_cmd)
    {
        if(choice_pipe_setup(vars) == 1)
			return ;
        if (vars->nb_cmd >= 2 || tmp->infile_fd > 2 || tmp->outfile_fd > 2 || is_builtins_exec(vars) == 0)
            parent_process(vars, tmp, envp);
		tmp = tmp->next;
		vars->cmd_index++;
	}
}

int    fork_processes(t_vars *vars, t_redirection **redirect, t_env **envp)
{
	initialize_vars(vars);
	process_commands(vars, redirect, envp);
    wait_process(vars);
    ft_close_fd(vars);
    ft_lstclear_final_redirection(redirect);
    ft_free_tab_3d(vars);
    return (0);
}
