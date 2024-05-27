/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:41:21 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/27 13:22:24 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_strlen(char const *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char *s2)
{
	char	*tab;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (0);
	tab = malloc((1 + ft_strlen(s1) + ft_strlen(s2)) * sizeof(char));
	if (!tab)
		return (NULL);
	i = 0;
	j = 0;
	while (s1 && s1[i])
	{
		tab[i] = s1[i];
		i++;
	}
	while (s2 && s2[j])
	{
		tab[i] = s2[j];
		j++;
		i++;
	}
	tab[i] = 0;
	return (tab);
}

char	*slash_add(char const *s1, char *s2)
{
	char	*tab;
	int		i;
	int		j;

	tab = malloc((1 + ft_strlen(s1) + ft_strlen(s2)) * sizeof(char));
	if (!tab)
		return (NULL);
	i = 0;
	j = 0;
	while (s1 && s1[i])
	{
		tab[i] = s1[i];
		i++;
	}
	while (s2 && s2[j])
	{
		tab[i] = s2[j];
		j++;
		i++;
	}
	tab[i] = 0;
	if (s2)
		free(s2);
	return (tab);
}

int	ft_strchr(char *s, int c)
{
	int	i;

	if (!s)
		return (0);
	if (c > 255)
		c = c % 256;
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (i + 1);
		i++;
	}
	return (0);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	char		*tab;
	size_t		i;
	long int	size_m;

	size_m = nmemb * size;
	if ((int)size_m < 0 || ((int)size < 0 && (int)nmemb < 0))
		return (NULL);
	i = 0;
	tab = malloc(size_m);
	if (tab == 0)
		return (NULL);
	while (i < (nmemb * size))
	{
		tab[i] = '\0';
		i++;
	}
	return (tab);
}
