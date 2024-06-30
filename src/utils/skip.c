/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:24:57 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/29 15:59:39 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*skip_spaces(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\n' || str[i] == '\t') || str[i] == ' ')
			i++;
		else
			break ;
	}
	return ((char *)&str[i]);
}

char	*skip_one_character(const char *str)
{
	return ((char *)&str[1]);
}

char	*skip_quote(
		const char *str, char c, t_argument_parsing *result)
{
	int	i;

	i = 0;
	if (str[i] == c)
		i++;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return ((char *)&str[i + 1]);
	else
	{
		ft_putstr_fd("Error: quotes are not closed !\n", 2);
		result->did_succeed = FALSE;
		return (NULL);
	}
}

int	skip_dolar_var(char *argument, int index)
{
	int	i;

	i = index;
	i++;
	while (argument[i] != 0
		&& argument[i] != ' ' && argument[i] != '\n'
		&& argument[i] != '\t' && argument[i] != '|'
		&& argument[i] != '<' && argument[i] != '>'
		&& argument[i] != '\'' && argument[i] != '"'
		&& argument[i] != '$')
		i++;
	return (i);
}
