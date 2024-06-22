/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skips_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 11:25:17 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/22 11:25:31 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	skip_spaces_and_tabs(char *str)
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
