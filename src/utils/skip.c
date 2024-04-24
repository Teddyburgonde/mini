/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:24:57 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/24 15:45:59 by rgobet           ###   ########.fr       */
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
		if ((str[i] == NEW_LINE || str[i] == TAB) || str[i] == SPACE)
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
		const char *str, char c, t_argument_parsing_result *result)
{
	int	i;

	i = 0;
	if (i == 0 && str[i] == c)
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
	while (argument[i] != 0
		&& argument[i] != SPACE && argument[i] != NEW_LINE &&
		argument[i] != TAB && argument[i] != PIPE &&
		argument[i] != LEFT && argument[i] != RIGHT &&
		argument[i] != '\'' && argument[i] != '"')
		i++;
	return (i);
}
