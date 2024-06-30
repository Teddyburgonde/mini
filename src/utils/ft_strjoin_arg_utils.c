/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_arg_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 15:07:39 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/30 10:57:31 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*allocate_and_prepare_tab(const char *s1, const char *s2)
{
	char	*tab;
	size_t	len1;
	size_t	len2;

	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strcspn(s2, "<>|'\" \n\t");
	tab = malloc((1 + len1 + len2) * sizeof(char));
	return (tab);
}

void	copy_and_concatenante(char *tab, const char *s1, const char *s2)
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
		while (ft_strcspn(s2, "<>|'\" \n\t") > j)
		{
			tab[i] = s2[j];
			i++;
			j++;
		}
	}
	tab[i] = 0;
}
