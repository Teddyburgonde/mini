/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:15:04 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/25 14:47:12 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_var_name(char *str)
{
	char	*var_name;
	int		i;

	i = 1;
	var_name = malloc((strcspn(&str[i], "<>\'\"| \n\t$") + 2) * sizeof(char));
	if (!var_name)
		return (NULL);
	if (str[0])
		var_name[0] = str[0];
	while (str[i] && str[i - 1] && str[i] != ' ' && str[i] != '\n'
		&& str[i] != '\t' && str[i] != '|' && str[i] != '<'
		&& str[i] != '>' && str[i] != '\''
		&& str[i] != '"' && str[i] != '$')
	{
		var_name[i] = str[i];
		i++;
	}
	var_name[i] = 0;
	return (var_name);
}

static t_argument	*ft_expand_vars_in_argument(
		const char *argument, t_env *env, t_vars *vars)
{
	t_char_list	*tmp;
	t_argument	*arg;
	t_bool		in_quote;
	t_env		*var;
	char		*var_name;
	char		*exit_code;
	int			i;
	int			j;
	int			s;

	i = 0;
	exit_code = NULL;
	in_quote = FALSE;
	arg = lst_new_argument();
	if (!arg || !argument)
		return (NULL);
	j = 0;
	while (argument[i] != 0)
	{
		s = 0;
		if (argument[i] == '\'' && in_quote == FALSE)
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
			i++;
		}
		else if (argument[i] == '"' || (in_quote == TRUE && argument[i] != '$'))
		{
			if (argument[i] == '"' && in_quote == FALSE)
			{
				tmp = lst_new_char_list();
				if (!tmp)
					return (NULL);
				in_quote = TRUE;
				tmp->value = argument[i];
				ft_lstadd_back_char_list(&arg->chars, tmp);
				i++;
			}
			while (argument[i] && argument[i] != '"')
			{
				if (argument[i] == '$')
					break ;
				tmp = lst_new_char_list();
				if (!tmp)
					return (NULL);
				tmp->value = argument[i];
				ft_lstadd_back_char_list(&arg->chars, tmp);
				i++;
			}
			if (argument[i] == '"')
			{
				tmp = lst_new_char_list();
				if (!tmp)
					return (NULL);
				in_quote = FALSE;
				tmp->value = argument[i];
				ft_lstadd_back_char_list(&arg->chars, tmp);
				i++;
			}
		}
		else if (argument[i] == '$')
		{
			// I added argument[i + 1] != '?'to get code error
			var_name = get_var_name((char *)&argument[i]);
			if (ft_strcmp(var_name, "$?") == 0
				|| (var_name[0] == '$' && var_name[1] == '?'))
			{
				exit_code = ft_itoa(vars->exit_code);
				while (exit_code[s])
				{
					tmp = lst_new_char_list();
					if (!tmp)
						return (NULL);
					tmp->value = exit_code[s];
					tmp->was_in_a_variable = TRUE;
					ft_lstadd_back_char_list(&arg->chars, tmp);
					s++;
				}
				free(exit_code);
				i += 2;
			}
			else if (lst_search_env(var_name, env))
			{
				var = lst_search_env(var_name, env);
				while (var->value[s])
				{
					tmp = lst_new_char_list();
					if (!tmp)
						return (NULL);
					tmp->value = var->value[s];
					tmp->was_in_a_variable = TRUE;
					ft_lstadd_back_char_list(&arg->chars, tmp);
					s++;
				}
				i = skip_dolar_var((char *)argument, i);
			}
			else if ((argument[i] == '$' && argument[i + 1] == 0)
				|| (argument[i] == '$' && argument[i + 1] == '"'
					&& in_quote == TRUE)
				|| ((argument[i] == '$' && (argument[i + 1] == ' '
				|| argument[i + 1] == '\t' || argument[i + 1] == '\n'))))
			{
				tmp = lst_new_char_list();
				if (!tmp)
					return (NULL);
				tmp->value = argument[i];
				tmp->was_in_a_variable = FALSE;
				ft_lstadd_back_char_list(&arg->chars, tmp);
				i = skip_dolar_var((char *)argument, i);
			}
			else
				i = skip_dolar_var((char *)argument, i);
			if (var_name)
				free(var_name);
		}
		else
		{
			while (argument[i])
			{
				// I added argument[i + 1] != '?' to get code error
				if (argument[i] == '$')
					break ;
				tmp = lst_new_char_list();
				if (!tmp)
					return (NULL);
				tmp->value = argument[i];
				ft_lstadd_back_char_list(&arg->chars, tmp);
				i++;
			}
		}
		if (argument[i] != 0 && argument[i] != '$' && s == 0)
			i++;
	}
	return (arg);
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

static int	ft_split_argument(t_argument *argument_to_split,
	t_argument **args)
{
	int			in;
	char		quote;
	t_char_list	*arg;
	t_char_list	*tmp_char;
	t_argument	*tmp;
	t_argument	*splitted_arguments;
	t_bool		in_quote;
	t_bool		quote_in_var;

	quote = 0;
	splitted_arguments = lst_new_argument();
	if (!splitted_arguments)
		return (0);
	in = 0;
	arg = NULL;
	quote_in_var = FALSE;
	tmp = argument_to_split;
	tmp = ft_get_last_pos(tmp);
	if (tmp == NULL)
		tmp = argument_to_split;
	if (tmp->chars->value == '"'
		|| tmp->chars->value == '\'')
	{
		quote = tmp->chars->value;
		in_quote = TRUE;
	}
	else
		in_quote = FALSE;
	tmp_char = tmp->chars;
	tmp_char->last_pos = FALSE;
	while (tmp_char && in_quote == FALSE)
	{
		if ((tmp_char->value == '\'' || tmp_char->value == '"')
			&& quote_in_var == FALSE)
		{
			quote = tmp_char->value;
			quote_in_var = TRUE;
		}
		else if ((tmp_char->value == '\'' || tmp_char->value == '"')
			&& quote_in_var == TRUE)
			quote_in_var = FALSE;
		if (quote_in_var == FALSE
			&& (tmp_char->value == ' '
				|| tmp_char->value == '\t'
				|| tmp_char->value == '\n'))
			break ;
		if (tmp_char->value == '\'' || tmp_char->value == '"')
			tmp_char = tmp_char->next;
		if (tmp_char && tmp_char->value != quote)
		{
			arg = lst_new_char_list();
			if (!arg)
				return (0);
			arg->value = tmp_char->value;
			arg->was_in_a_variable = tmp_char->was_in_a_variable;
			ft_lstadd_back_char_list(&splitted_arguments->chars, arg);
			tmp_char->last_pos = FALSE;
			tmp_char = tmp_char->next;
		}
	}
	while (tmp_char && in_quote == TRUE)
	{
		if (tmp_char->value == '\''
			|| tmp_char->value == '"')
		{
			if (in == 1 && tmp_char->value == quote)
				in_quote = FALSE;
			else if (in == 0)
			{
				quote = tmp_char->value;
				in++;
			}
		}
		if (tmp_char->value && tmp_char->next && quote == tmp_char->value
			&& ((tmp_char->value == '\'' && tmp_char->next->value == '\'')
				|| (tmp_char->value == '"' && tmp_char->next->value == '"')))
			tmp_char = tmp_char->next->next;
		if (tmp_char && tmp_char->value != quote)
		{
			arg = lst_new_char_list();
			if (!arg)
				return (0);
			arg->value = tmp_char->value;
			arg->was_in_a_variable = tmp_char->was_in_a_variable;
			ft_lstadd_back_char_list(&splitted_arguments->chars, arg);
			tmp_char->last_pos = FALSE;
			tmp_char = tmp_char->next;
		}
		else if (tmp_char)
			tmp_char = tmp_char->next;
	}
	while (tmp_char && tmp_char->value != ' '
		&& tmp_char->value != '\t' && tmp_char->value != '\n')
	{
		arg = lst_new_char_list();
		if (!arg)
			return (0);
		arg->value = tmp_char->value;
		arg->was_in_a_variable = tmp_char->was_in_a_variable;
		arg->next = NULL;
		ft_lstadd_back_char_list(&splitted_arguments->chars, arg);
		tmp_char->last_pos = FALSE;
		tmp_char = tmp_char->next;
	}
	ft_lstadd_back_argument(args, splitted_arguments);
	if (tmp_char == NULL)
	{
		tmp = tmp->next;
		if (tmp != NULL)
			tmp_char = tmp->chars;
	}
	if (tmp == NULL)
		return (0);
	tmp_char->last_pos = TRUE;
	return (1);
}

t_argument	*ft_expand_argument(const t_argument_to_expand *argument,
		t_env *env, t_vars *vars)
{
	t_argument				*argument_with_expanded_vars;
	t_argument				*args_with_expanded_vars;
	t_argument				*splitted_arguments;
	t_argument				*tmp_split;
	t_argument_to_expand	*tmp_to_expand;
	int						tmp;

	tmp = 1;
	tmp_to_expand = (t_argument_to_expand *)argument;
	args_with_expanded_vars = NULL;
	while (tmp_to_expand != NULL)
	{
		argument_with_expanded_vars = ft_expand_vars_in_argument(
				tmp_to_expand->content, env, vars);
		if (argument_with_expanded_vars->chars == NULL)
		{
			free(argument_with_expanded_vars);
			argument_with_expanded_vars = NULL;
			break ;
		}
		ft_lstadd_back_argument(&args_with_expanded_vars,
			argument_with_expanded_vars);
		tmp_to_expand = tmp_to_expand->next;
	}
	splitted_arguments = NULL;
	if (args_with_expanded_vars != NULL)
	{
		while (tmp != 0)
			tmp = ft_split_argument(args_with_expanded_vars, &splitted_arguments);
		tmp_split = splitted_arguments;
		// if (tmp_split->chars)
		// 	ft_remove_quotes(&tmp_split);
	}
	if (args_with_expanded_vars)
		ft_lstclear_argument(&args_with_expanded_vars);
	else
		return (NULL);
	return (splitted_arguments);
}
