/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 11:20:34 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/12 11:31:27 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Envoie parsing_result afin d'avoir acces aux redirections ainsi que l'env,
// dans la fonction on appel ft_expand argument sur chaque commande.
// Puis appel de ft_setup commande qui permet d'obtenir un char **.
// On regarde si la commande est un built-in si oui on l'envoie 
// dans un selecteur de buit-in. Le selecteur execute le built-in en question.
// Si se n'est pas un built-in on creer un fork ... (pipex_bonus)

static void	process_successful_command(t_command_line_parsing_result
*parsing_result, t_env **env, char *command_line)
{
	ft_cmd_manager(env, parsing_result);
	ft_lstclear_commands(&parsing_result->commands);
	free(parsing_result);
	free(command_line);
}

static int skip_spaces_and_tabs(char *str)
{
	int i;
	
	i = 0;
	while (str[i])
	{
		if ((str[i] >= 9 && str[i] <= 13) || str[i] == 32 || str[i] == '\n')
			i++;
		else
			return (1);
	}
	return (0);
}

static void	verif_command_line(char *command_line
	, t_env **env)
{
	t_command_line_parsing_result	*parsing_result;

	if (command_line != NULL && command_line[0] != '\0')
	{
		add_history(command_line);
		parsing_result = ft_parse_command_line(command_line);
		if (skip_spaces_and_tabs(command_line) == 0)
			return ;
		if (!parsing_result->did_succeed)
		{
			free(command_line);
			ft_lstclear_commands(&parsing_result->commands);
			free(parsing_result);
		}
		else if (command_line[0] == '|')
		{
			write (2, "Error: too much pipe or in a wrong position !\n", 46);
			free(command_line);
		}
		else if (ft_strcmp(command_line, "''") == 0
			|| ft_strcmp(command_line, "\"\"") == 0)
		{
			write(2, "Command '' not found, but can be installed with !\n", 50);
			ft_lstclear_commands(&parsing_result->commands);
			free(parsing_result);
			free(command_line);
		}
		else if (parsing_result->commands->arguments != NULL && (ft_strcmp(
					parsing_result->commands->arguments->content, "\"\"") == 0
				|| ft_strcmp(parsing_result->commands->arguments->content,
					"''") == 0) && parsing_result->commands->next == NULL)
		{
			// erfe ?????????
			printf("erfe");
			write(2, "Command '' not found, but can be installed with !\n", 50);
			ft_lstclear_commands(&parsing_result->commands);
			free(parsing_result);
			free(command_line);
		}
		else
			process_successful_command(parsing_result, env, command_line);
	}
}

int	ft_readline(t_env **env)
{
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
		verif_command_line(command_line, env);
	}
	return (0);
}
