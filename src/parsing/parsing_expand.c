/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:15:04 by rgobet            #+#    #+#             */
/*   Updated: 2024/04/24 15:45:24 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


char	*get_var_name(char *str)
{
	char	*var_name;
	int		i;

	i = 0;
	var_name = malloc((strcspn(str, "<>\'\"| \n\t") + 1) * sizeof(char));
	if (!var_name)
		return (NULL);
	while (str[i] && str[i] != SPACE && str[i] != NEW_LINE
		&& str[i] != TAB && str[i] != PIPE && str[i] != LEFT
		&& str[i] != RIGHT && str[i] != '\'' && str[i] != '"')
	{
		var_name[i] = str[i];
		i++;
	}
	var_name[i] = 0;
	return (var_name);
}

static t_argument	*ft_expand_vars_in_argument(
		const char *argument, t_env *env)
{
	t_char_list	*tmp;
	t_argument	*arg;
	t_env		*var;
	int			i;
	int			j;

	i = 0;
	arg = lst_new_argument();
	if (!arg || !argument)
		return (NULL);
	while (argument[i] != 0)
	{
		j = 0;
		if (argument[i] == '\'')
		{
			tmp = lst_new_char_list();
			if (!tmp)
				return (NULL);
			tmp->value = argument[i];
			ft_lstadd_back_char_list(&arg->chars, tmp);
			i++;
			while (argument[i] != '\'')
			{
				tmp = lst_new_char_list();
				if (!tmp)
					return (NULL);
				tmp->value = argument[i];
				ft_lstadd_back_char_list(&arg->chars, tmp);
				i++;
			}
			tmp = lst_new_char_list();
			if (!tmp)
				return (NULL);
			tmp->value = argument[i];
			ft_lstadd_back_char_list(&arg->chars, tmp);
		}
		else
		{
			if (argument[i] == '$'
				&& lst_search_env(get_var_name((char *)&argument[i]), env))
			{
				var = lst_search_env(get_var_name((char *)&argument[i]), env);
				while (var->var[j])
				{
					tmp = lst_new_char_list();
					if (!tmp)
						return (NULL);
					tmp->value = env->var[j];
					tmp->was_in_a_variable = TRUE;
					ft_lstadd_back_char_list(&arg->chars, tmp);
				}
			}
			else
				i = skip_dolar_var((char *)argument, i);
		}
		i++;
	}
	return (arg);
}

static void	ft_split_argument(const t_argument *argument_to_split,
	t_argument **args)
{
	t_char_list	*arg;
	t_argument	*splitted_arguments;
	t_argument	*actual_arg;
	t_bool		in_quote;

	splitted_arguments = lst_new_argument();
	if (!splitted_arguments)
		return ;
	actual_arg = (t_argument *)argument_to_split;
	if (actual_arg->chars->value == '"' || actual_arg->chars->value == '\'')
		in_quote = TRUE;
	else
		in_quote = FALSE;
	while (actual_arg->chars && in_quote == FALSE)
	{
		if (actual_arg->chars->value == SPACE || actual_arg->chars->value == TAB
			|| actual_arg->chars->value == NEW_LINE)
			break ;
		arg = lst_new_char_list();
		arg->value = actual_arg->chars->value;
		arg->was_in_a_variable = actual_arg->chars->was_in_a_variable;
		ft_lstadd_back_char_list(&splitted_arguments->chars, arg);
		actual_arg->chars = actual_arg->chars->next;
	}
	while (actual_arg->chars && in_quote == TRUE)
	{
		if (actual_arg->chars->value == '\''
			|| actual_arg->chars->value == '"')
			in_quote = FALSE;
		arg = lst_new_char_list();
		arg->value = actual_arg->chars->value;
		arg->was_in_a_variable = actual_arg->chars->was_in_a_variable;
		ft_lstadd_back_char_list(&splitted_arguments->chars, arg);
		actual_arg->chars = actual_arg->chars->next;
	}
	ft_lstadd_back_argument(args, splitted_arguments);
	if (actual_arg->chars->next == NULL)
		actual_arg = actual_arg->next;
}

static void	ft_remove_quotes(t_char_list *src)
{
	t_char_list	*tmp;
	t_char_list	*tmp2;

	tmp = src;
	src = src->next;
	free(tmp);
	tmp = src;
	while (tmp->next != NULL)
	{
		if (tmp->next->next == NULL)
			tmp2 = tmp;
		tmp = tmp->next;
	}
	free(tmp);
	tmp2->next = NULL;
}

t_argument	*ft_expand_argument(const t_argument_to_expand *argument,
		t_env *env)
{
	t_argument			*argument_with_expanded_vars;
	t_argument			*splitted_arguments;
	t_argument			*tmp_split;
	t_argument			*tmp;

	argument_with_expanded_vars = ft_expand_vars_in_argument(
			argument->content, env);
	tmp = argument_with_expanded_vars;
	while (tmp->chars != NULL)
		ft_split_argument(tmp, &splitted_arguments);
	tmp_split = splitted_arguments;
	while (tmp_split)
	{
		while (tmp_split->chars)
		{
			if (tmp_split->chars->value == '\''
				|| tmp_split->chars->value == '"')
				ft_remove_quotes(splitted_arguments->chars);
			tmp_split->chars = tmp_split->chars->next;
		}
		tmp_split = tmp_split->next;
	}
	return (splitted_arguments);
}

// Modif et supp t_splitted_argument car faut utiliser t_argument
// Chaque arg = un node et chaque node de t_char_list = une lettre