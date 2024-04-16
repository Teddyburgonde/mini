/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 14:24:05 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/16 14:50:52 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	double_redirection(char *str)
{
	str = skip_one_character(str);
	str = skip_one_character(str);
	str = skip_spaces(str);
	if (str[0] == '>' || str[0] == '<')
	{
		ft_putstr_fd("syntax error near unexpected token `newline'", 2);
		return (1);
	}
	return (0);
}

int	single_redictection(char *str)
{
	str = skip_one_character(str);
	str = skip_spaces(str);
	if (str[0] == '>' || str[0] == '<')
	{
		ft_putstr_fd("syntax error near unexpected token `newline'", 2);
		return (1);
	}
	return (0);
}

int  ft_strcspn(char *s, char *reject)
{
    int	i;
    int	j;

	i = 0;
    while (s[i] != '\0')
    {
		j = 0;
        while (reject[j] != '\0')
        {
            if(s[i] == reject[j])
                return (i);
            j++;
        }
        i++;
    }
    return (i);
}
