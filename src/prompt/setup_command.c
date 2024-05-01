/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:00:27 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/01 05:11:19 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_lstsize_modif(t_argument *argument)
{
	int	counter;

	counter = 0;
	if (!argument)
		return (0);
	while (argument != NULL)
	{
		argument = argument->next;
		counter++;
	}
	return (counter);
}

static int	ft_lstsize_char(t_char_list *chars)
{
	int	counter;

	counter = 0;
	if (!chars)
		return (0);
	while (chars != NULL)
	{
		chars = chars->next;
		counter++;
	}
	return (counter);
}

static char	*assemble(t_char_list *tmp_char)
{
	t_char_list	*tmp;
	char		*str;
	int			i;
	int			char_list_size;

	char_list_size = ft_lstsize_char(tmp_char);
	tmp = tmp_char;
	str = malloc((char_list_size + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	tmp_char = tmp;
	i = 0;
	while (tmp_char != NULL)
	{
		str[i] = tmp_char->value;
		tmp_char = tmp_char->next;
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	**ft_setup_command(t_argument *arg)
{
	char		**str;
	t_argument	*tmp_arg;
	t_char_list	*tmp_char;
	int			j;

	j = 0;
	str = malloc(((ft_lstsize_modif(arg) + 1)) * sizeof(char *));
	tmp_arg = arg;
	while (tmp_arg != NULL)
	{
		tmp_char = tmp_arg->chars;
		str[j] = assemble(tmp_char);
		tmp_arg = tmp_arg->next;
		j++;
	}
	str[j] = NULL;
	return (str);
}
