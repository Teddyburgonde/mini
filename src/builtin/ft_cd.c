/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:21:16 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/29 07:56:01 by tebandam         ###   ########.fr       */
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
	if (check_error_ft_cd_home(command) == 1)
		return (EXIT_FAILURE);
	if (command[1] != NULL && command[2] != NULL)
	{
		write (2, " too many arguments\n", 20);
		return (1);
	}
	return (0);
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
		write (2, " No such file or directory\n", 27);
		exit(EXIT_FAILURE);
	}
	return (stock);
}

int	ft_cd(char **command, t_env **env)
{
	char	*path_current;
	char	*stock;
	t_env	*current;

	current = *env;
	if (check_error_ft_cd(command) == 1)
		return (1);
	current = find_env_by_var_name(*env, "OLDPWD");
	path_current = getcwd(NULL, 0);
	free(current->full_path);
	current->full_path = ft_strjoin("OLDPWD=", path_current);
	free(path_current);
	current = *env;
	current = find_env_by_var_name(*env, "PWD");
	stock = command[1];
	if (command[1])
		stock = ft_chdid_and_verif(stock);
	free(current->full_path);
	path_current = getcwd(NULL, 0);
	current->full_path = ft_strjoin("PWD=", path_current);
	free(path_current);
	return (EXIT_SUCCESS);
}
