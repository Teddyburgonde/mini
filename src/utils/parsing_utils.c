/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:28:15 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/21 16:39:07 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_strjoin_arg(char *s1, const char *s2)
{
	char	*tab;

	if (!s1 && !s2)
		return (0);
	tab = allocate_and_prepare_tab(s1, s2);
	if (!tab)
		return (NULL);
	copy_and_concatenante(tab, s1, s2);
	if (s1)
		free(s1);
	return (tab);
}

char	*ft_strjoin_file(char *s1)
{
	char	*tab;
	int		len;

	if (!s1)
		return (0);
	tab = allocate_tab(s1, &len);
	if (!tab)
		return (NULL);
	copy_content(tab, s1, len);
	return (tab);
}


// static char	*ft_allocate_tab(const char *s1, const char *s2, char *reject)
// {
// 	char *tab;
// 	size_t	len1;
// 	size_t	len2;
	
// 	len1 = 0;
// 	len2 = 0;
// 	if (s1)
// 		len1 = ft_strlen(s1);
// 	if (s2)
// 		len2 = ft_strcspn(s2, reject);
// 	tab = malloc((1 + len1 + len2) * sizeof(char));
// 	return (tab);
// }

// static void	copy_tab(char *tab, const char *s1, const char *s2, char *reject)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	j = 0;
// 	if (s1)
// 	{
// 		while (s1[i])
// 		{
// 			tab[i] = s1[i];
// 			i++;
// 		}
// 	}
// 	if (s2)
// 	{
// 		while (j < ft_strcspn(s2, reject))
// 		{
// 			tab[i] = s2[j];
// 			i++;
// 			j++;
// 		}
// 	}
// 	tab[i] = 0;
// }

char	*ft_strjoin_until(char *s1, const char *s2, char *reject)
{
	char	*tab;
	
	if (!s1 && !s2)
		return (0);
	tab = ft_allocate_tab(s1, s2, reject);
	if (!tab)
		return (NULL);
	copy_tab(tab, s1, s2, reject);
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
		return (NULL);
	i = 0;
	while (s1 && s1[i])
	{
		tab[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && (ft_strcspn(&s2[1], reject) + 2) > j)
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
