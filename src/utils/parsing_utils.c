/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:28:15 by tebandam          #+#    #+#             */
/*   Updated: 2024/04/17 14:37:56 by tebandam         ###   ########.fr       */
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

char	*ft_strjoin_arg(char *s1, const char *s2)
{
	char	*tab;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (0);
	tab = malloc((1 + ft_strlen(s1) + ft_strcspn(s2, "<>\'\"| \n\t")) * sizeof(char));
	if (tab == 0)
		return (0);
	i = 0;
	while (s1 && s1[i])
	{
		tab[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && ft_strcspn(s2, "<>\'\"| \n\t") > j)
	{
		tab[i] = s2[j++];
		i++;
	}
	tab[i] = 0;
	if (s1)
		free(s1);
	return (tab);
}

char	*ft_strjoin_until(char *s1, const char *s2, char *reject)
{
	char	*tab;
	int		i;
	int		j;

	if (s1 == NULL && s2 == NULL)
		return (0);
	tab = malloc((1 + ft_strlen(s1) + ft_strcspn(
					s2, reject)) * sizeof(char));
	if (tab == 0)
		return (0);
	i = 0;
	while (s1 && s1[i])
	{
		tab[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && ft_strcspn(s2, reject) > j)
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

char	*ft_strjoin_quoted_arg(char *s1, const char *s2, char *reject)
{
	char	*tab;
	int		i;
	int		j;

	if (s1 == NULL && s2 == NULL)
		return (0);
	tab = malloc((3 + ft_strlen(s1) + ft_strcspn(
					&s2[1], reject)) * sizeof(char));
	if (tab == 0)
		return (0);
	i = 0;
	while (s1 && s1[i])
	{
		tab[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && (ft_strcspn(&s2[1], reject) + 1) > j)
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
		ft_putstr_fd("Error\n", 2);
		result->did_succeed = FALSE;
		return (NULL);
	}
}
