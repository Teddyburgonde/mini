/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_expand_in_argument.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:20:40 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/30 15:20:27 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	*add_char_in_double_quotes(const char *argument, int *i,
	t_char_list **chars)
{
	t_char_list	*tmp;

	tmp = NULL;
	while (argument[*i] && argument[*i] != '"')
	{
		if (argument[*i] == '$')
			break ;
		tmp = lst_new_char_list();
		if (!tmp)
			return (NULL);
		tmp->value = argument[*i];
		ft_lstadd_back_char_list(chars, tmp);
		*i += 1;
	}
	return ((void *)1);
}

static void	*expand_argument_double_quote(const char *argument, int *i,
	t_bool in_quote, t_char_list **chars)
{
	t_char_list	*tmp;

	if (argument[*i] == '"' && in_quote == FALSE)
	{
		tmp = lst_new_char_list();
		if (!tmp)
			return (NULL);
		in_quote = TRUE;
		tmp->value = argument[*i];
		ft_lstadd_back_char_list(chars, tmp);
		*i += 1;
	}
	if (add_char_in_double_quotes(argument, i, chars) == NULL)
		return (NULL);
	if (argument[*i] == '"')
	{
		tmp = lst_new_char_list();
		if (!tmp)
			return (NULL);
		in_quote = FALSE;
		tmp->value = argument[*i];
		ft_lstadd_back_char_list(chars, tmp);
		*i += 1;
	}
	return ((void *)1);
}

static void	*expand_argument_single_quote(const char *argument, int *i,
	t_char_list **chars)
{
	t_char_list	*tmp;

	tmp = NULL;
	tmp = lst_new_char_list();
	if (!tmp)
		return (NULL);
	tmp->value = argument[*i];
	ft_lstadd_back_char_list(chars, tmp);
	*i += 1;
	while (argument[*i] != '\'')
	{
		tmp = lst_new_char_list();
		if (!tmp)
			return (NULL);
		tmp->value = argument[*i];
		ft_lstadd_back_char_list(chars, tmp);
		*i += 1;
	}
	tmp = lst_new_char_list();
	if (!tmp)
		return (NULL);
	tmp->value = argument[*i];
	ft_lstadd_back_char_list(chars, tmp);
	*i += 1;
	return ((void *)1);
}

static void	*simple_arg(char *argument, int *i, t_char_list **chars)
{
	t_char_list	*tmp;

	tmp = NULL;
	while (argument[*i])
	{
		if (argument[*i] == '$')
			break ;
		tmp = lst_new_char_list();
		if (!tmp)
			return (NULL);
		tmp->value = argument[*i];
		ft_lstadd_back_char_list(chars, tmp);
		*i += 1;
	}
	return ((void *)1);
}

t_argument	*ft_expand_vars_in_argument(
		const char *argument, t_env *env, t_vars *vars)
{
	t_argument	*arg;
	int			i;

	i = 0;
	arg = lst_new_argument();
	if (!arg || !argument)
		return (NULL);
	while (argument && argument[i])
	{
		if (argument[i] == '\'' && is_in_quote((char *)argument, i) == TRUE)
			expand_argument_single_quote(argument, &i, &arg->chars);
		else if (argument[i] == '"'
			|| (is_in_quote((char *)argument, i) == TRUE && argument[i] != '$'))
			expand_argument_double_quote(argument, &i,
				is_in_quote((char *)argument, i), &arg->chars);
		else if (argument[i] == '$')
			i += expand_argument((char *)&argument[i],
					env, vars, &arg->chars);
		else
			simple_arg((char *)argument, &i, &arg->chars);
		if (argument[i] != 0 && argument[i] != '$' && argument[i - 1] != '$'
			&& need_to_increment((char *)argument, i) == FALSE)
			i++;
	}
	return (arg);
}
