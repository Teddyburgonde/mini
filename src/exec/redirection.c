/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:54:32 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/25 16:28:40 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_flow_redirection(t_vars *vars, t_redirection *redirect)
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
		else if (vars->cmd_index % 2 == 0)
			last_command_redirection_even(redirect, vars);
	}
}

t_bool	is_last_infile(t_redirection_to_expand *tmp_redirection)
{
	tmp_redirection = tmp_redirection->next;
	while (tmp_redirection)
	{
		if (tmp_redirection->e_type == REDIRECTION_HEREDOC
			|| tmp_redirection->e_type == REDIRECTION_INFILE)
			return (FALSE);
		tmp_redirection = tmp_redirection->next;
	}
	return (TRUE);
}

static int	refresh_nb_heredoc(int nb_heredoc, t_redirection *redirection)
{
	if (redirection)
		return (redirection->nb_heredoc);
	return (nb_heredoc);
}

static void	manage_redirection(t_redirection **result,
	t_redirection *redirection, t_bool heredoc)
{
	if (!redirection)
		*result = no_redirection();
	ft_lstadd_back_redirection(result, redirection);
	where_are_heredoc(result, heredoc);
}

void	stock_redirection(t_command_to_expand *list, t_env *env,
	t_vars *vars, t_redirection **result)
{
	t_command_to_expand		*tmp_command;
	t_redirection_to_expand	*tmp_redirection;
	t_redirection			*redirection;
	t_bool					heredoc;
	int						nb_hd;

	tmp_command = list;
	init (&heredoc, &nb_hd, result, &redirection);
	while (tmp_command)
	{
		nb_hd = refresh_nb_heredoc(nb_hd, redirection);
		redirection_setup(&redirection, nb_hd);
		tmp_redirection = tmp_command->redirections;
		while (tmp_redirection && redirection)
		{
			heredoc = global_redirections(redirection,
					tmp_redirection, vars);
			if (heredoc == TRUE)
				complete_heredoc(redirection, tmp_command, env, vars);
			tmp_redirection = tmp_redirection->next;
		}
		manage_redirection(result, redirection, heredoc);
		tmp_command = tmp_command->next;
	}
	security_clear(&list);
}
