/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 11:20:34 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/30 13:11:26 by rgobet           ###   ########.fr       */
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
	t_argument						*final_parsing;
	char							*command_line;

	while (1)
	{
		command_line = readline("minishell > ");
		if (command_line == NULL)
		{
			free(command_line);
			break ;
		}
		add_history(command_line);
		parsing_result = ft_parse_command_line(command_line);
		if (!parsing_result->did_succeed)
		{
			// handle error
		}
		ft_cmd_manager(env, parsing_result);
		ft_lstclear_commands(&parsing_result->commands);
		ft_lstclear_argument(&final_parsing);
		free(parsing_result);
		free(command_line);
	}
	return (0);
}
