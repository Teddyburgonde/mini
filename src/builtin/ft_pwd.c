/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:37:00 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/18 08:22:12 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_pwd(t_vars *vars, t_redirection *redirect)
{
	char	buf[1024];

	if (vars->nb_cmd == 1)
	{
		if (getcwd(buf, sizeof(buf)) == NULL)
		{
			ft_putstr_fd("An error has been detected", 2);
			// exit_code 1 ( valeur non null si il y a un probleme)
			return (1);
		}
		ft_putstr_fd(buf, redirect->outfile_fd);
		write (redirect->outfile_fd, "\n", 1);
	}
	else
	{
		if (getcwd(buf, sizeof(buf)) == NULL)
		{
			ft_putstr_fd("An error has been detected", 2);
			// exit_code 1 ( valeur non null si il y a un probleme)
			return (1);
		}
		ft_putstr_fd(buf, 1);
		write (1, "\n", 1);
	}
	// exit_code 0 si tout se passe bien
	return (0);
}
