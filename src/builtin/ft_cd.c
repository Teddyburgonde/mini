/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:21:16 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/26 23:41:05 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_error_ft_cd_home(char **command)
{
	const char	*home;

	home = getenv("HOME");
	if (command[1] == NULL)
	{
		if (home == NULL)
		{
			perror("The HOME environment variable is not defined.\n");
			return (EXIT_FAILURE);
		}
		if (chdir(home) != 0)
		{
			perror("Error when changing directory.\n");
			return (EXIT_FAILURE);
		}
	}
	return (0);
}

int	check_error_ft_cd(char **command)
{
	const char	*home;

	home = NULL;
	if (check_error_ft_cd_home(command) == 1)
		return (EXIT_FAILURE);
	if (command[2] != NULL)
	{
		write (1, "Error\n", 6);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

t_env	*find_env_by_var_name(t_env *env, const char *var_name)
{
	t_env	*current;

	current = env;
	while (current != NULL)
	{
		if (strcmp(current->var_name, var_name) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

char	*ft_chdid_and_verif(char *stock)
{
	if (chdir(stock) == -1)
	{
		printf("error directory\n");
		exit(EXIT_FAILURE);
	}
	return (stock);
}

int	ft_cd(char **command, char **envp)
{
	char	*path_current;
	char	*stock;
	t_env	*env;
	t_env	*current;

	env = NULL;
	init_env(&env, envp);
	if (env == NULL)
		return (1);
	current = env;
	check_error_ft_cd(command);
	current = find_env_by_var_name(env, "OLDPWD");
	path_current = getcwd(NULL, 0);
	free(current->full_path);
	current->full_path = ft_strjoin("OLDPWD=", path_current);
	current = env;
	current = find_env_by_var_name(env, "PWD");
	stock = command[1];
	stock = ft_chdid_and_verif(stock);
	free(current->full_path);
	current->full_path = ft_strjoin("PWD=", getcwd(NULL, 0));
	return (EXIT_SUCCESS);
}
