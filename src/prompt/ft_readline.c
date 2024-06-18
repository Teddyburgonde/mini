/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 11:20:34 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/18 14:37:59 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Envoie parsing_result afin d'avoir acces aux redirections ainsi que l'env,
// dans la fonction on appel ft_expand argument sur chaque commande.
// Puis appel de ft_setup commande qui permet d'obtenir un char **.
// On regarde si la commande est un built-in si oui on l'envoie 
// dans un selecteur de buit-in. Le selecteur execute le built-in en question.
// Si se n'est pas un built-in on creer un fork ... (pipex_bonus)

static char	*process_successful_command(t_command_line_parsing_result
*parsing_result, t_env **env, char *command_line, t_vars *vars)
{
	ft_cmd_manager(env, parsing_result, vars);
	ft_lstclear_commands(&parsing_result->commands);
	free(parsing_result);
	if (command_line)
		free(command_line);
	command_line = NULL;
	return (command_line);
}

static int skip_spaces_and_tabs(char *str)
{
	int	i;

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

static char	*verif_command_line(char *command_line
	, t_env **env, t_vars *vars)
{
	t_command_line_parsing_result	*parsing_result;

	if (command_line != NULL && command_line[0] != '\0')
	{
		add_history(command_line);
		parsing_result = ft_parse_command_line(command_line);
		if (skip_spaces_and_tabs(command_line) == 0)
		{
			free(parsing_result);
			free(command_line);
			command_line = NULL;
			vars->exit_code = 2;
			return (command_line);
		}
		if (!parsing_result->did_succeed)
		{
			free(command_line);
			command_line = NULL;
			vars->exit_code = 2;
			ft_lstclear_commands(&parsing_result->commands);
			free(parsing_result);
		}
		else if (command_line[0] == '|')
		{
			write (2, "Error: too much pipe or in a wrong position !\n", 46);
			vars->exit_code = 2;
			free(command_line);
			command_line = NULL;
		}
		else if (ft_strcmp(command_line, "''") == 0
			|| ft_strcmp(command_line, "\"\"") == 0)
		{
			write(2, "Command '' not found, but can be installed with !\n", 50);
			ft_lstclear_commands(&parsing_result->commands);
			vars->exit_code = 2;
			free(parsing_result);
			free(command_line);
			command_line = NULL;
		}
		else if (parsing_result->commands->arguments != NULL && (ft_strcmp(
					parsing_result->commands->arguments->content, "\"\"") == 0
				|| ft_strcmp(parsing_result->commands->arguments->content,
					"''") == 0) && parsing_result->commands->next == NULL)
		{
			write(2, "Command '' not found, but can be installed with !\n", 50);
			ft_lstclear_commands(&parsing_result->commands);
			free(parsing_result);
			vars->exit_code = 2;
			free(command_line);
			command_line = NULL;
		}
		else
			command_line = process_successful_command(
					parsing_result, env, command_line, vars);
		return (command_line);
	}
	return (command_line);
}

int	ft_readline(t_env **env, t_vars *vars)
{
	char							*command_line;

	command_line = NULL;
	while (1)
	{
		if (command_line)
			free(command_line);
		command_line = NULL;
		command_line = readline("minishell ~ ");
		if (command_line == NULL)
		{
			free(command_line);
			break ;
		}
		command_line = verif_command_line(command_line, env, vars);
	}
		printf("%d\n", vars->exit_code);
	return (0);
}
