/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:15:04 by rgobet            #+#    #+#             */
/*   Updated: 2024/04/24 11:19:27 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
		const char* argument, t_env *env)
{
	t_char_list	*tmp;
	t_argument	*arg;
	t_env		*var;
	int			i;
	int			j;

	i = 0;
	arg = lst_new_argument();
	if (!arg)
		return (NULL);
	while (argument[i])
	{
		j = 0;
		if (argument[i] == '\'')
		{
			tmp = lst_new_chars_list();
			if (!tmp)
				return (NULL);
			tmp->value = argument[i];
			ft_lstadd_back_char_list(&arg->chars, tmp);
			i++;
			while (argument[i] != '\'')
			{
				tmp = lst_new_chars_list();
				if (!tmp)
					return (NULL);
				tmp->value = argument[i];
				ft_lstadd_back_char_list(&arg->chars, tmp);
				i++;
			}
			tmp = lst_new_chars_list();
			if (!tmp)
				return (NULL);
			tmp->value = argument[i];
			ft_lstadd_back_char_list(&arg->chars, tmp);
		}
		else
		{
			if (argument[i] == '$'
				&& lst_search_env(get_var_name(&argument[i]), env))
			{
				var = lst_search_env(get_var_name(&argument[i]), env);
				while (var->var[j])
				{
					tmp = lst_new_chars_list();
					if (!tmp)
						return (NULL);
					tmp->value = env->var[j];
					tmp->was_in_a_variable = TRUE;
					ft_lstadd_back_char_list(&arg->chars, tmp);
				}
			}
			else
				i = skip_dolar_var(argument, i);
		}
		i++;
	}
	return (arg);
}

static t_argument	*ft_split_argument(const t_argument *argument_to_split,
	t_splitted_argument **args)
{
	t_splitted_argument	*splitted_arguments;
	t_argument			*actual_arg;
	t_bool				in_quote;
	int					i;

	splitted_argument = lst_new_splitted_argument();
	if (!splitted_arguments)
		return (NULL);
	actual_arg = argument_to_split;
	splitted_arguments->argument = malloc(
			(1 + ft_lstsize_expand(actual_arg->chars)) * sizeof(char));
	if (actual_arg->chars->value == '"' || actual_arg->chars->value == '\'')
		in_quote = TRUE;
	else
		in_quote = FALSE;
	i = 0;
	while (actual_arg->chars && in_quote == FALSE)
	{
		if (actual_arg->chars->value == SPACE || actual_arg->chars->value == TAB
			|| actual_arg->chars->value == NEW_LINE)
			break ;
		splitted_arguments->argument[i] = actual_arg->chars->value;
		actual_arg->chars = actual_arg->chars->next;
		i++;
	}
	while (actual_arg->chars && in_quote == TRUE)
	{
		if (actual_arg->chars->value == '\''
			|| actual_arg->chars->value == '"')
			in_quote = FALSE;
		splitted_arguments->argument[i] = actual_arg->chars->value;
		actual_arg->chars = actual_arg->chars->next;
		i++;
	}
	splitted_arguments->argument[i] = 0;
	ft_lstadd_back_splitted_argument(args, splitted_arguments);
	if (actual_arg->chars->next == NULL)
		actual_arg = actual_arg->next;
	return (actual_arg);
}

static char	*ft_remove_quotes(char *src)
{
	char	*dest;
	int		i;
	int		j;

	i = 1;
	j = 0;

	dest = malloc(sizeof(char *) + ft_strlen(src) - 1);
	while (src[i])
	{
		dest[j] = src[i];
		i++;
		j++;
		if (src[i + 1] == '\0')
			break ;
	}
	dest[j] = '\0';
	return (dest);
}

t_argument	*ft_expand_argument(const t_argument_to_expand argument, t_env *env)
{
	t_argument	*argument_with_expanded_vars;
	t_argument	*splitted_arguments;
	t_argument	*final_arguments;
	t_argument	*tmp;

	argument_with_expanded_vars = ft_expand_vars_in_argument(
			argument.content, env);
	tmp = argument_with_expanded_vars;
	while (tmp->chars->value != 0)
		splitted_arguments = ft_split_argument(tmp);
	final_arguments = ft_remove_quotes_from_arguments(splitted_arguments);
	return (final_arguments);
}