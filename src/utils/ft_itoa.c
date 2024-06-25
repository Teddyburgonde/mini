/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:14:06 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/24 15:14:47 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static size_t	search_leng(int n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		len++;
	while (n)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static void	fill_str(char *str, int n, size_t len)
{
	size_t	i;

	str[len] = '\0';
	i = 0;
	if (n < 0)
	{
		str[0] = '-';
		i = 1;
	}
	while (len > i)
	{
		len--;
		if (n < 0)
		{
			str[len] = n % 10 * (-1) + '0';
			n = n / 10;
		}
		else
		{
			str[len] = n % 10 + '0';
			n = n / 10;
		}
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	len;

	len = search_leng(n);
	str = malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	fill_str(str, n, len);
	return (str);
}
