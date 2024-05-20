/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:24:50 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/20 12:59:10 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_strjoin_mod(char *s1, char *s2)
{
	char	*tab;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j = 0;
	tab = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!tab)
		return (NULL);
	while (s1[i])
	{
		tab[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		tab[i + j] = s2[j];
		j++;
	}
	tab[i + j] = '\0';
	free(s1);
	s1 = NULL;
	return (tab);
}

char	*read_loop(char *buf, char *stock, int *len, int fd)
{
	buf = malloc(sizeof(char) * BUFFER_SIZE + 1);
	*len = read(fd, buf, BUFFER_SIZE);
	if (*len == -1 || (*len == 0 && stock[0] == '\0'))
	{
		free(buf);
		free(stock);
		return (NULL);
	}
	buf[*len] = 0;
	stock = ft_strjoin_mod(stock, buf);
	free(buf);
	buf = NULL;
	return (stock);
}

char	*ft_strchr_rl(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)(&s[i]));
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)(&s[i]));
	return (NULL);
}

char	*read_line(int fd, char *stock)
{
	char	*buf;
	int		len;

	len = 1;
	buf = NULL;
	if (stock != NULL)
	{
		buf = malloc(sizeof(char) * BUFFER_SIZE + 1);
		len = read(fd, buf, BUFFER_SIZE);
		buf[len] = 0;
		if (len > 0)
			stock = ft_strjoin_mod(stock, buf);
		free(buf);
		buf = NULL;
	}
	else
	{
		stock = malloc(sizeof(char));
		stock[0] = 0;
	}
	while (len > 0 && ft_strchr_rl(stock, '\n') == NULL)
		stock = read_loop(buf, stock, &len, fd);
	free(buf);
	return (stock);
}
char	*ft_strdup_gnl(const char *s)
{
	char		*tab;
	int			i;

	i = 0;
	tab = malloc(sizeof(char) * ft_strlen(s) + 1);
	if (tab)
	{
		while (s[i])
		{
			tab[i] = s[i];
			i++;
		}
		tab[i] = '\0';
	}
	return ((char *)tab);
}



