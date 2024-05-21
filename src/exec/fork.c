/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:03:38 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/21 16:13:06 by tebandam         ###   ########.fr       */
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

static void	ft_flow_redirection(t_vars *vars, t_redirection *redirect)
{
	// if (current_cmd == 0)
	write(2, "BITCH", 5);
		// Envoi dans le pipe pour l'afficher dans process parent / renvoyer
		// if (redirect->outfile_fd != STDOUT_FILENO)
		// 	dup2(redirect->outfile_fd, STDOUT_FILENO);
		// else
		// 	dup2(vars->pipe_1[1], STDOUT_FILENO);
		// dup2(redirect->infile_fd, STDIN_FILENO);


	// Added
	// Si infile == STDOUT et que y a un resultat dans le pipe qui est l'outfile ????
	// Meme dans tout les cas
	if (redirect->e_type == PIPE_OUT)
	{
		if (dup2(vars->pipe_1[1], STDOUT_FILENO) < 0)
			perror("dup2");
	}
	else
	{
		if (dup2(redirect->outfile_fd, STDOUT_FILENO) < 0)
			perror("dup2");
	}
	// Si infile == STDIN et que y a un resultat dans le pipe qui est l'infile ????
	// Meme dans tout les cas
	if (nb == 0)
	{
		if (dup2(redirect->infile_fd, STDIN_FILENO) < 0)
			perror("dup2");
	}
	else
	{
		if (dup2(vars->pipe_1[0], STDIN_FILENO) < 0)
			perror("dup2");
	}
	// else if (actual_cmd == vars->nb_cmd - 1)
	// {
	// 	write(2, "BIATCH", 5);
	// 	if (dup2(vars->tmp_fd, STDIN_FILENO) < 0)
	// 		perror("dup2");
	// 	if (dup2(redirect->outfile_fd, STDOUT_FILENO) < 0)
	// 		perror("dup2");
	// }
	// else
	// {
	// 	write(2, "BIATCHOU", 5);
	// 	if (dup2(vars->tmp_fd, STDIN_FILENO) < 0)
	// 		perror("dup2");
	// 	if (dup2(vars->pipe_1[1], STDOUT_FILENO) < 0)
	// 		perror("dup2");
	// }
}

static int	child_process(t_vars *vars, t_redirection *redirect
		, char **actual_cmd)
{

	ft_flow_redirection(vars, redirect);
	ft_putstr_fd("\n\n\n\n", 2);
	ft_putstr_fd(ft_itoa(vars->tmp_fd), 2);
	if (redirect->infile_fd == -1 || redirect->outfile_fd == -1)
	{
		write(2, "BITCHOUUUUUUUUUUUUUUUUUUUU", 5);
		perror("Error opening files");
		exit(1);
	}
	if (vars->tmp_fd != -1)
		close(vars->tmp_fd);
	close(vars->pipe_1[0]);
	close(vars->pipe_1[1]);
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

int	fork_processes(t_vars *vars, t_command_to_expand *tmp, t_redirection *redirect)
{
	int	i;

	i = 0;
	while (i < vars->nb_cmd)
	{
		// open_files(vars, tmp->redirections);
		printf("out :%i\n", redirect->outfile_fd);
		printf("in :%i\n", redirect->infile_fd);
		printf("nb_cmd :%i\n", vars->nb_cmd);
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
			// Cree une lecture sur le file du pipe car le pipe va etre delete
			vars->tmp_fd = dup(vars->pipe_1[0]);
			write(2, "JAJA\n", 5);
			close(vars->pipe_1[0]);
			close(vars->pipe_1[1]);
		}
		tmp = tmp->next;
		redirect = redirect->next;
		i++;
	}
	return (0);
}