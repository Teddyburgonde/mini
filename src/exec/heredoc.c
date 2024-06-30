/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:11:14 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/30 14:29:37 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	error_heredoc(char *tmp_content, int count, t_vars *vars)
{
	char	*exit_code;

	exit_code = ft_itoa(count);
	free(tmp_content);
	ft_putstr_fd("bash: warning: here-document at line ", 2);
	ft_putstr_fd(exit_code, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `:')\n", 2);
	vars->exit_code = 0;
	free(exit_code);
}

static char	*fill_tmp_content(char *tmp_content, t_redirection *redirection,
	t_env *env, t_vars *vars)
{
	static int		count;
	t_redirection	*tmp_redirection;

	count++;
	tmp_redirection = redirection;
	tmp_content = readline("> ");
	if (tmp_content == NULL)
	{
		error_heredoc(tmp_content, count, vars);
		return (tmp_content);
	}
	if (ft_strcmp(tmp_content, tmp_redirection->limiter) == 0)
	{
		free(tmp_content);
		tmp_content = NULL;
		return (tmp_content);
	}
	else
	{
		if (need_to_be_expand_hd(tmp_content, env) >= TRUE)
			tmp_content = expand_line(tmp_content, env, vars);
		ft_putstr_fd(tmp_content, redirection->infile_fd);
		ft_putstr_fd("\n", redirection->infile_fd);
	}
	return (tmp_content);
}

static int	fill_useless_heredocs(char **tmp_content, int count, t_vars *vars)
{
	if (*tmp_content)
		free(*tmp_content);
	*tmp_content = NULL;
	*tmp_content = readline("> ");
	if (*tmp_content == NULL)
	{
		error_heredoc(*tmp_content, count, vars);
		return (1);
	}
	return (0);
}

static char	*last_heredoc(char *tmp_content, t_redirection *redirection,
	t_env *env, t_vars *vars)
{
	while (1)
	{
		if (tmp_content)
			free(tmp_content);
		tmp_content = NULL;
		tmp_content = fill_tmp_content(tmp_content, redirection,
				env, vars);
		if (!tmp_content)
			break ;
	}
	if (tmp_content)
		free(tmp_content);
	return (NULL);
}

void	ft_heredoc(t_redirection *redirection,
		t_redirection_to_expand *all,
		t_env *env, t_vars *vars)
{
	char			*tmp;
	int				count;

	while (all)
	{
		tmp = NULL;
		if (all->e_type == REDIRECTION_HEREDOC)
		{
			count = 0;
			while (ft_strcmp(tmp, all->arg) != 0 || count == 0)
			{
				if (redirection->e_position == HERE && is_last(all) == all)
					tmp = last_heredoc(tmp, redirection, env, vars);
				else
				{
					if (fill_useless_heredocs(&tmp, count, vars) == 1)
						break ;
				}
				count++;
			}
			free(tmp);
		}
		all = all->next;
	}
	exit(vars->exit_code);
}
