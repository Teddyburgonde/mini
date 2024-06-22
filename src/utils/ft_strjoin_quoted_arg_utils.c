/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_quoted_arg_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 19:46:09 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/21 19:52:10 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*allocate_tab_quoted(const char *s1, const char *s2, char *reject)
{
	char	*tab;
	size_t	len1;
	size_t	len2;

	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strcspn(&s2[1], reject) + 2;
	tab = malloc((len1 + len2 + 1) * sizeof(char));
	if (!tab)
		return (NULL);
	return (tab);
}

void	copy_and_concatenate_quoted_arg(char *tab, const char *s1,
	const char *s2, char *reject)
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
		while (j < ft_strcspn(&s2[1], reject) + 2)
		{
			tab[i] = s2[j];
			j++;
			i++;
		}
	}
	tab[i] = '\0';
}
