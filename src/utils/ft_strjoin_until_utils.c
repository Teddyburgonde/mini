/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_until_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:38:53 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/21 19:49:17 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_allocate_tab(const char *s1, const char *s2, char *reject)
{
	char	*tab;
	size_t	len1;
	size_t	len2;

	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strcspn(s2, reject);
	tab = malloc((1 + len1 + len2) * sizeof(char));
	return (tab);
}

void	copy_tab(char *tab, const char *s1, const char *s2, char *reject)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (s1)
	{
		while (s1[i])
		{
			tab[i] = s1[i];
			i++;
		}
	}
	if (s2)
	{
		while (j < ft_strcspn(s2, reject))
		{
			tab[i] = s2[j];
			i++;
			j++;
		}
	}
	tab[i] = 0;
}
