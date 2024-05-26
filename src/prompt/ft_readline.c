/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 11:20:34 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/26 11:36:46 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Envoie parsing_result afin d'avoir acces aux redirections ainsi que l'env,
// dans la fonction on appel ft_expand argument sur chaque commande.
// Puis appel de ft_setup commande qui permet d'obtenir un char **.
// On regarde si la commande est un built-in si oui on l'envoie 
// dans un selecteur de buit-in. Le selecteur execute le built-in en question.
// Si se n'est pas un built-in on creer un fork ... (pipex_bonus)

int	ft_readline(t_env **env)
{
	t_command_line_parsing_result	*parsing_result;
	char							*command_line;

	command_line = NULL;
	while (1)
	{
		command_line = readline("minishell ~ ");
		if (command_line == NULL)
		{
			free(command_line);
			break ;
		}
		add_history(command_line);
		parsing_result = ft_parse_command_line(command_line);
		if (!parsing_result->did_succeed)
		{
			free(command_line);
			ft_lstclear_commands(&parsing_result->commands);
			free(parsing_result);
		}
		ft_cmd_manager(env, parsing_result);
		ft_lstclear_commands(&parsing_result->commands);
		free(parsing_result);
		free(command_line);
	}
	return (0);
}
