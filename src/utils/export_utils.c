/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 20:54:30 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/30 15:34:16 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static size_t	ft_min(const char *s, size_t len)
{
	if (len > (size_t)ft_strlen(s))
		return ((size_t)ft_strlen(s));
	return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*tab;

	i = start;
	if (!s)
		return (0);
	if (start >= (unsigned int)ft_strlen(s))
	{
		tab = ft_calloc(1, sizeof(char));
		if (tab == 0)
			return (0);
	}
	else
	{
		len = ft_min(&s[start], len);
		tab = ft_calloc((1 + len), sizeof(char));
		if (tab == 0)
			return (0);
		while (s[i] && len > (i - start))
		{
			tab[i - start] = s[i];
			i++;
		}
	}
	return (tab);
}
