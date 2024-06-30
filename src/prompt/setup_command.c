/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:00:27 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/29 20:40:59 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_lstsize_modif(t_argument *argument)
{
	int	counter;

	counter = 0;
	while (argument != NULL)
	{
		argument = argument->next;
		counter++;
	}
	return (counter);
}

char	**ft_setup_command(t_argument *arg)
{
	char		**str;
	t_argument	*tmp_arg;
	t_char_list	*tmp_char;
	int			char_list_size;
	int			size;
	int			i;
	int			j;

	i = 0;
	j = 0;
	char_list_size = 0;
	size = ft_lstsize_modif(arg);
	str = malloc(((size + 1)) * sizeof(char *));
	tmp_arg = arg;
	while (tmp_arg != NULL)
	{
		tmp_char = tmp_arg->chars;
		char_list_size = 0;
		while (tmp_char != NULL)
		{
			char_list_size++;
			tmp_char = tmp_char->next;
		}
		str[j] = malloc((char_list_size + 1) * sizeof(char));
		if (str[j] == NULL)
			return (NULL);
		tmp_char = tmp_arg->chars;
		i = 0;
		while (tmp_char != NULL)
		{
			str[j][i] = tmp_char->value;
			tmp_char = tmp_char->next;
			i++;
		}
		str[j][i] = '\0';
		j++;
		tmp_arg = tmp_arg->next;
	}
	str[j] = NULL;
	return (str);
}
