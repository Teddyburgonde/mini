/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 18:50:02 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/30 15:13:12 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_bool	is_in_quote(char *str, int i)
{
	int		j;
	t_bool	in_quote;

	j = 0;
	in_quote = FALSE;
	while (str && str[j])
	{
		if (str[j] == '\'' && in_quote == FALSE)
			in_quote = TRUE;
		else if (str[j] == '\'' && in_quote == TRUE)
			in_quote = FALSE;
		if (j == i)
			return (in_quote);
		j++;
	}
	return (in_quote);
}

void	refresh_quotes_status(t_bool *in_quote, char current)
{
	if (current != 0)
	{
		if (*in_quote && *in_quote == TRUE && current == '\'')
			*in_quote = FALSE;
		else if (*in_quote && *in_quote == FALSE && current == '\'')
			*in_quote = TRUE;
	}
}
