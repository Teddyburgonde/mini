/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:28:15 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/13 14:12:51 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char *skip_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] == NEW_LINE || str[i] == TAB) || str[i] == SPACE)
			i++;
		else
			break ;
	}
	return (&str[i]);
}

char	*skip_one_character(char *str)
{
	return (&str[1]);
}

static int	ft_find_fist_word(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] != SPACE && str[i] != TAB && str[i] != NEW_LINE
		&& str[i] != '"' && str[i] != '\'')
		i++;
	return (i + 1);
}

char	*ft_strjoin_arg(char *s1, char *s2)
{
	char	*tab;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (0);
	tab = malloc((1 + ft_strlen(s1) + ft_find_fist_word(s2)) * sizeof(char));
	if (tab == 0)
		return (0);
	i = 0;
	while (s1 && s1[i])
	{
		tab[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j] != SPACE && s2[j] != NEW_LINE && s2[j] != TAB
		&& s2[j] != '"' && s2[j] != '\'')
	{
		tab[i] = s2[j];
		j++;
		i++;
	}
	tab[i] = 0;
	if (s1)
		free(s1);
	return (tab);
}

char	*ft_strjoin_until(char *s1, char *s2, char c)
{
	char	*tab;
	int		i;
	int		j;
	int		n;

	if (s1 == NULL && s2 == NULL)
		return (0);
	if (ft_strchr(&s2[1], c) == 0)
	{
		tab = malloc((1 + ft_strlen(s1) + ft_strchr_exception(&s2[1]))
				* sizeof(char));
	}
	else
		tab = malloc((3 + ft_strlen(s1) + ft_strchr(&s2[1], c)) * sizeof(char));
	if (tab == 0)
		return (0);
	i = 0;
	n = 0;
	while (s1 && s1[i])
	{
		tab[i] = s1[i];
		i++;
	}
	j = 0;
	if (ft_strchr(&s2[1], c) != 0)
	{
		while (s2 && n != 2)
		{
			if (s2[j] != c)
				n++;
			tab[i] = s2[j];
			j++;
			i++;
		}
	}
	else
	{
		while (ft_strchr_exception(&s2[1]) > j)
		{
			if (s2[j] == c)
				n++;
			tab[i] = s2[j];
			j++;
			i++;
		}
	}
	tab[i] = 0;
	if (s1)
		free(s1);
	return (tab);
}

char	*skip_quote(
		char *str, char c, t_argument_parsing_result *result)
{
	int	i;

	i = 0;
	if (i == 0 && str[i] == c)
		i++;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (&str[i]);
	else
	{
		ft_putstr_fd("Error\n", 2);
		result->did_succeed = FALSE;
		return (NULL);
	}
}
