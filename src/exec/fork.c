/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:03:38 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/20 15:43:37 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <unistd.h>


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

int	child_process(t_vars *vars, t_redirection *redirect, char **actual_cmd)
{
	ft_putstr_fd("\n\n\n\n", 2);
	ft_putstr_fd(ft_itoa(redirect->infile_fd), 2); 
	if (redirect->infile_fd == -1 || redirect->outfile_fd == -1)
	{
		perror("Error opening files");
		exit(1);
	}
	dup2(redirect->infile_fd, STDIN_FILENO);
	dup2(redirect->outfile_fd, STDOUT_FILENO);
	if (vars->tmp_fd != -1)
		close(vars->tmp_fd);
	close(vars->pipe_1[0]);
	close(vars->pipe_1[1]);
	close(redirect->infile_fd);
	close(redirect->outfile_fd);
	execve(actual_cmd[0], actual_cmd, vars->env);
	perror("Execve");
	ft_free(vars->path);
	ft_free_tab_3d(vars);
	return (1);
}

int	fork_processes(t_vars *vars, t_command_to_expand *tmp, t_redirection *redirect)
{
	int	i;

	i = 0;
	while (i < vars->nb_cmd)
	{
		// open_files(vars, tmp->redirections);
		if (pipe(vars->pipe_1) == -1)
			return (EXIT_FAILURE);
		vars->child = fork();
		if (vars->child == 0)
			child_process(vars, redirect, vars->cmd[i]);
		else if (vars->child < 0)
		{
			perror("fork");
			return (EXIT_FAILURE);
		}
		else
		{
			if (vars->tmp_fd != -1)
				close(vars->tmp_fd);
			vars->tmp_fd = dup(vars->pipe_1[0]);
			close(vars->pipe_1[0]);
			close(vars->pipe_1[1]);
		}
		tmp = tmp->next;
		i++;
	}
	return (0);
}