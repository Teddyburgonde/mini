/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 14:24:05 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/14 15:00:43 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Modif le message d'erreur avec le char actuel 
// PS : c'est str[0]

int	double_redirection(char *str)
{
	str = skip_one_character(str);
	str = skip_one_character(str);
	str = skip_spaces(str);
	if (str[0] == '\0')
	{
		ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
		return (1);
	}
	if (str[0] == '>')
	{
		ft_putstr_fd("syntax error near unexpected token `>'\n", 2);
		return (1);
	}
	if (str[0] == '<')
	{
		ft_putstr_fd("syntax error near unexpected token `<'\n", 2);
		return (1);
	}
	return (0);
}

int	single_redirection(char *str)
{
	str = skip_one_character(str);
	str = skip_spaces(str);
	if (str[0] == '\0')
	{
		ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
		return (1);
	}
	if (str[0] == '>')
	{
		ft_putstr_fd("syntax error near unexpected token `>'\n", 2);
		return (1);
	}
	if (str[0] == '<')
	{
		ft_putstr_fd("syntax error near unexpected token `<'\n", 2);
		return (1);
	}
	return (0);
}

int	ft_strcspn(const char *s, char *reject)
{
	int	i;
	int	j;

	i = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
	{
		j = 0;
		while (reject[j] != '\0')
		{
			if (s[i] == reject[j])
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}

int	ft_strcspn2(const char *s, char *reject)
{
	int	i;
	int	j;

	i = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
	{
		j = 0;
		while (reject[j] != '\0')
		{
			if (s[i] == reject[j])
				return (i);
			j++;
		}
		i++;
	}
	return (0);
}

char	*ft_skip_arg(const char *str, char *reject)
{
	while (ft_strcspn(str, reject))
		str++;
	return ((char *)str);
}
