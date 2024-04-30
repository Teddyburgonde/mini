/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_copy.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:06:33 by rgobet            #+#    #+#             */
/*   Updated: 2024/04/30 11:28:00 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*copy(char *s)
{
	int		i;
	char	*tmp;

	tmp = malloc(sizeof(char) * ft_strlen(s) + 1);
	if (!tmp)
		return (NULL);
	i = 0;
	while (s[i])
	{
		tmp[i] = s[i];
		i++;
	}
	tmp[i] = 0;
	return (tmp);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	mult;
	int	result;

	i = 0;
	mult = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-')
	{
		mult = -1;
		i++;
	}
	else if (str[i] == '+' && mult == 1)
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * mult);
}
