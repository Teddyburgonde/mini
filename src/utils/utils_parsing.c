/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:28:15 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/02 17:37:25 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

const char *skip_spaces(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] == NEW_LINE || str[i] == TAB) || str[i] == SPACE)
			i++;
		else
			break;
	}
	return (&str[i]);
}

const char	*skip_one_character(const char *str)
{
	return (&str[1]);
}
