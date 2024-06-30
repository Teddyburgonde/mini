/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_split_argument_utils.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 20:01:11 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/30 15:25:28 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_bool	quote_function(t_char_list **tmp_char,
	char *quote, t_bool in_quote)
{
	static int	in = 0;

	if (!*quote)
		in = 0;
	if ((*tmp_char)->value == '\''
		|| (*tmp_char)->value == '"')
	{
		if (in == 1 && (*tmp_char)->value == *quote)
		{
			in_quote = FALSE;
			in = 0;
		}
		else if (in == 0)
		{
			*quote = (*tmp_char)->value;
			in++;
		}
	}
	if ((*tmp_char)->value && (*tmp_char)->next && *quote == (*tmp_char)->value
		&& (((*tmp_char)->value == '\'' && (*tmp_char)->next->value == '\'')
			|| ((*tmp_char)->value == '"' && (*tmp_char)->next->value == '"')))
		*tmp_char = (*tmp_char)->next->next;
	return (in_quote);
}

void	fill_no_quote_arg(t_char_list **tmp_char,
	t_char_list **splitted_chars, char quote)
{
	t_char_list	*arg;

	arg = NULL;
	if (tmp_char && (*tmp_char)->value != quote)
	{
		arg = lst_new_char_list();
		if (!arg)
			return ;
		arg->value = (*tmp_char)->value;
		arg->was_in_a_variable = (*tmp_char)->was_in_a_variable;
		ft_lstadd_back_char_list(splitted_chars, arg);
		(*tmp_char)->last_pos = FALSE;
		*tmp_char = (*tmp_char)->next;
	}
}

int	function_verif_quote(t_char_list **tmp_char, char *quote,
	t_bool *quote_in_var)
{
	if (((*tmp_char)->value == '\'' || (*tmp_char)->value == '"')
		&& *quote_in_var == FALSE)
	{
		*quote = (*tmp_char)->value;
		*quote_in_var = TRUE;
		return (1);
	}
	else if (((*tmp_char)->value == '\'' || (*tmp_char)->value == '"')
		&& *quote_in_var == TRUE)
		*quote_in_var = FALSE;
	if (*quote_in_var == FALSE
		&& ((*tmp_char)->value == ' '
			|| (*tmp_char)->value == '\t'
			|| (*tmp_char)->value == '\n'))
		return (1);
	if ((*tmp_char)->next && *quote_in_var == TRUE
		&& *quote == (*tmp_char)->value
		&& *quote == (*tmp_char)->next->value)
		*tmp_char = (*tmp_char)->next->next;
	else if ((*tmp_char)->value == '\'' || (*tmp_char)->value == '"')
		*tmp_char = (*tmp_char)->next;
	return (0);
}

static t_argument	*ft_get_last_pos(t_argument *lst)
{
	t_char_list	*tmp;

	while (lst)
	{
		tmp = lst->chars;
		while (tmp)
		{
			if (tmp->last_pos == TRUE)
				return (lst);
			tmp = tmp->next;
		}
		lst = lst->next;
	}
	return (NULL);
}

t_bool	init_function(char *quote, t_argument **tmp,
	t_argument	**splitted_arguments, t_argument *argument_to_split)
{
	t_bool	in_quote;

	in_quote = FALSE;
	*quote = 0;
	*splitted_arguments = lst_new_argument();
	if (!*splitted_arguments)
		return (0);
	*tmp = argument_to_split;
	*tmp = ft_get_last_pos(*tmp);
	if (*tmp == NULL)
		*tmp = argument_to_split;
	if ((*tmp)->chars->value == '"'
		|| (*tmp)->chars->value == '\'')
	{
		*quote = (*tmp)->chars->value;
		in_quote = TRUE;
	}
	else
		in_quote = FALSE;
	return (in_quote);
}
