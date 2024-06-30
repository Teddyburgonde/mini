/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_split_argument.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:56:30 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/29 20:06:56 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	rest_argument(t_char_list	**tmp_char,
	t_char_list **splitted_chars)
{
	t_char_list	*arg;

	arg = NULL;
	while (*tmp_char && (*tmp_char)->value != ' '
		&& (*tmp_char)->value != '\t' && (*tmp_char)->value != '\n')
	{
		arg = lst_new_char_list();
		if (!arg)
			return ;
		arg->value = (*tmp_char)->value;
		arg->was_in_a_variable = (*tmp_char)->was_in_a_variable;
		arg->next = NULL;
		ft_lstadd_back_char_list(splitted_chars, arg);
		(*tmp_char)->last_pos = FALSE;
		*tmp_char = (*tmp_char)->next;
	}
}

static void	fill_in_quote_arg(t_char_list **tmp_char,
	t_char_list **splitted_chars, char quote)
{
	t_char_list	*arg;

	arg = NULL;
	if (*tmp_char && (*tmp_char)->value != quote)
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
	else if (*tmp_char)
		*tmp_char = (*tmp_char)->next;
}

static int	set_last_point(t_argument **tmp, t_char_list **tmp_char)
{
	if (*tmp_char == NULL)
	{
		*tmp = (*tmp)->next;
		if (*tmp != NULL)
			*tmp_char = (*tmp)->chars;
	}
	if (*tmp == NULL)
		return (0);
	(*tmp_char)->last_pos = TRUE;
	return (1);
}

static void	set_var(t_argument *tmp, t_char_list **tmp_char)
{
	*tmp_char = tmp->chars;
	(*tmp_char)->last_pos = FALSE;
}

int	ft_split_argument(t_argument *argument_to_split,
	t_argument **args)
{
	char		quote;
	t_char_list	*tmp_char;
	t_argument	*tmp;
	t_argument	*splitted_arguments;
	t_bool		in_quote;

	in_quote = init_function(&quote, &tmp, &splitted_arguments,
			argument_to_split);
	set_var(tmp, &tmp_char);
	while (tmp_char && in_quote == FALSE)
	{
		if (function_verif_quote(&tmp_char, &quote, &in_quote) == 1)
			break ;
		fill_no_quote_arg(&tmp_char, &splitted_arguments->chars, quote);
	}
	while (tmp_char && in_quote == TRUE)
	{
		in_quote = quote_function(&tmp_char, &quote, in_quote);
		fill_in_quote_arg(&tmp_char, &splitted_arguments->chars, quote);
	}
	rest_argument(&tmp_char, &splitted_arguments->chars);
	ft_lstadd_back_argument(args, splitted_arguments);
	if (set_last_point(&tmp, &tmp_char) == 0)
		return (0);
	return (1);
}
