/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:20:30 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/29 16:47:44 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*copy(char *s)
{
	int		i;
	char	*tmp;

	if (!s)
		return (NULL);
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

static int	count_char(char c, char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}

char	*copy_without_quote(char *s)
{
	int		i;
	int		j;
	char	*tmp;

	if (!s)
		return (NULL);
	tmp = malloc(sizeof(char) * (ft_strlen(s) + 1
				- count_char('\'', s) - count_char('\"', s)));
	if (!tmp)
		return (NULL);
	i = 0;
	j = 0;
	while (s[j])
	{
		if (s[j] == '"' || s[j] == '\'')
			j++;
		if (s[j] != 0 && s[j] != '"' && s[j] != '\'')
		{
			tmp[i] = s[j];
			j++;
			i++;
		}
	}
	tmp[i] = 0;
	return (tmp);
}

char	*ft_strdup(char *s)
{
	char		*tab;
	int			i;

	i = 0;
	tab = malloc(sizeof(char) * ft_strlen(s) + 1);
	if (tab)
	{
		while (s && s[i])
		{
			tab[i] = s[i];
			i++;
		}
		tab[i] = '\0';
	}
	return ((char *)tab);
}
