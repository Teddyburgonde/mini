/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_file_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 15:00:23 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/21 19:53:58 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*allocate_tab(const char *s1, int *len)
{
	char	*tab;
	size_t	length;

	if (s1[0] != '\'' && s1[0] != '"')
	{
		length = ft_strcspn(s1, "<>| \n\t");
		tab = malloc((1 + length) * sizeof(char));
		*len = length;
	}
	else
	{
		length = ft_strcspn(&s1[1], "<>| \n\t");
		tab = malloc((3 + length) * sizeof(char));
		*len = length;
	}
	return (tab);
}

void	copy_content(char *tab, const char *s1, int len)
{
	int	i;

	i = 0;
	while (s1 && i < len)
	{
		tab[i] = s1[i];
		i++;
	}
	tab[i] = 0;
}
