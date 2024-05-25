/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:03:38 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/25 11:20:10 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <unistd.h>

// Sa degage V

static size_t	search_leng(int n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		len++;
	while (n)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static void	fill_str(char *str, int n, size_t len)
{
	size_t	i;

	str[len] = '\0';
	i = 0;
	if (n < 0)
	{
		str[0] = '-';
		i = 1;
	}
	while (len > i)
	{
		len--;
		if (n < 0)
		{
			str[len] = n % 10 * (-1) + '0';
			n = n / 10;
		}
		else
		{
			str[len] = n % 10 + '0';
			n = n / 10;
		}
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	len;

	len = search_leng(n);
	str = malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	fill_str(str, n, len);
	return (str);
}

// Sa degage /\bitch

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
	// printf("%i\n", vars->cmd_index);
	// Si premiere commande mais pas la derniere alors STDOUT -> Ecriture pipe et Le STDIN reste tel quel.
	if (vars->cmd_index == 1 && redirect->next != NULL)
	{
		if (redirect->infile_fd > 2)
		{
			if (dup2(redirect->infile_fd, STDIN_FILENO) < 0)
				perror("dup2");
		}
		if (redirect->outfile_fd > 2)
		{
			if (dup2(redirect->outfile_fd, STDOUT_FILENO) < 0)
				perror("dup2");
		}
		else if (vars->nb_cmd > 1)
		{
			if (dup2(vars->pipe_2[1], STDOUT_FILENO) < 0)
				perror("dup2");
		}
	}
	// Redirige les commandes dans les pipes pour les placer en stdin de la commande suivante
	else if (vars->cmd_index != 1 && redirect->next != NULL)
	{
		if (vars->cmd_index % 2 == 1)
		{
			if (redirect->infile_fd > 2)
			{
				if (dup2(redirect->infile_fd, STDIN_FILENO) < 0)
					perror("dup2");
			}
			else
			{
				if (dup2(vars->pipe_1[0], STDIN_FILENO) < 0)
					perror("dup2");
			}
			if (redirect->outfile_fd > 2)
			{
				if (dup2(redirect->outfile_fd, STDOUT_FILENO) < 0)
					perror("dup2");
			}
			else
			{
				if (dup2(vars->pipe_1[1], STDOUT_FILENO) < 0)
					perror("dup2");
			}
		}
		else
		{
			if (redirect->infile_fd > 2)
			{
				if (dup2(redirect->infile_fd, STDIN_FILENO) < 0)
					perror("dup2");
			}
			else
			{
				if (dup2(vars->pipe_2[0], STDIN_FILENO) < 0)
					perror("dup2");
			}
			if (redirect->outfile_fd > 2)
			{
				if (dup2(redirect->outfile_fd, STDOUT_FILENO) < 0)
					perror("dup2");
			}
			else
			{
				if (dup2(vars->pipe_2[1], STDOUT_FILENO) < 0)
					perror("dup2");
			}
		}
	}
	// Redirige la derniere commande vers le stdin de la derniere cmd et le resultat va dans STDOUT
	else if (redirect->next == NULL)
	{
		if (vars->cmd_index % 2 == 1)
		{
			if (redirect->infile_fd > 2)
			{
				if (dup2(redirect->infile_fd, STDIN_FILENO) < 0)
					perror("dup2");
			}
			else
			{
				if (dup2(vars->pipe_1[0], STDIN_FILENO) < 0)
					perror("dup2");
			}
			if (redirect->outfile_fd > 2)
			{
				if (dup2(redirect->outfile_fd, STDOUT_FILENO) < 0)
					perror("dup2");
			}
		}
		else
		{
			if (redirect->infile_fd > 2)
			{
				if (dup2(redirect->infile_fd, STDIN_FILENO) < 0)
					perror("dup2");
			}
			else
			{
				if (dup2(vars->pipe_2[0], STDIN_FILENO) < 0)
					perror("dup2");
			}
			if (redirect->outfile_fd > 2)
			{
				if (dup2(redirect->outfile_fd, STDOUT_FILENO) < 0)
					perror("dup2");
			}
		}
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
	execve(actual_cmd[0], actual_cmd, vars->env);
	perror("Execve");
	ft_free(vars->path);
	ft_free_tab_3d(vars);
	return (1);
}

/*
* Si heredoc/infile/outfile/append c'est la sortie/entree.
* Si pas de redirection envoie dans le pipe puis dans le parent STDOUT ou renvoie
* dans le pipe si redirect->e_type == PIPE_OUT
*/

#include <stdio.h>

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
	if ((vars->cmd_index - 1) % 2 == 1)
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
	else if ((vars->cmd_index - 1) % 2 == 0)
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
	while (waitpid(-1, NULL, 0) != -1)
		continue ;
	return (0);
}

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

int	fork_processes(t_vars *vars, t_redirection **redirect)
{
	t_redirection	*tmp;

	tmp = *redirect;
	vars->cmd_index = 1;
	vars->pipe_1[0] = -1;
	vars->pipe_1[1] = -1;
	vars->pipe_2[0] = -1;
	vars->pipe_2[1] = -1;
	while (vars->cmd_index <= vars->nb_cmd)
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
		tmp = tmp->next;
		vars->cmd_index++;
	}
	ft_close_fd(vars);
	ft_lstclear_final_redirection(redirect);
	ft_free_tab_3d(vars);
	return (0);
}
