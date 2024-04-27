/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:15:04 by rgobet            #+#    #+#             */
/*   Updated: 2024/04/27 04:22:55 by rgobet           ###   ########.fr       */
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
		else if (argument[i] == '$')
		{
			if (lst_search_env(get_var_name((char *)&argument[i]), env))
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
		else
		{
			while (argument[i])
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
		}
		if (argument[i] != 0)
			i++;
	}
	return (arg);
}

// Problem tmp2 is link to tmp .. But I need a var which keep the first node of each char_list

static void	ft_split_argument(t_argument **argument_to_split,
	t_argument **args)
{
	int			in;
	t_char_list	*arg;
	t_argument	*tmp;
	t_argument	*first_node;
	t_char_list	*next;
	t_argument	*splitted_arguments;
	t_bool		in_quote;

	splitted_arguments = lst_new_argument();
	if (!splitted_arguments)
		return ;
	in = 0;
	first_node = *argument_to_split;
	tmp = *argument_to_split;
	if (tmp->chars->value == '"'
		|| tmp->chars->value == '\'')
		in_quote = TRUE;
	else
		in_quote = FALSE;
	while (tmp->chars && in_quote == FALSE)
	{
		if (tmp->chars->value == SPACE
			|| tmp->chars->value == TAB
			|| tmp->chars->value == NEW_LINE)
			break ;
		arg = lst_new_char_list();
		arg->value = tmp->chars->value;
		arg->was_in_a_variable = tmp->chars->was_in_a_variable;
		ft_lstadd_back_char_list(&splitted_arguments->chars, arg);
		tmp->chars = tmp->chars->next;
	}
	while (tmp->chars && in_quote == TRUE)
	{
		if (tmp->chars->value == '\''
			|| tmp->chars->value == '"')
		{
			if (in == 1)
				in_quote = FALSE;
			else
				in++;
		}
		arg = lst_new_char_list();
		arg->value = tmp->chars->value;
		arg->was_in_a_variable = tmp->chars->was_in_a_variable;
		ft_lstadd_back_char_list(&splitted_arguments->chars, arg);
		tmp->chars = tmp->chars->next;
	}
	ft_lstadd_back_argument(args, splitted_arguments);
	if (tmp->chars == NULL)
	{
		tmp = tmp->next;
		*argument_to_split = (*argument_to_split)->next;
		while (first_node->chars)
		{
			next = first_node->chars->next;
			free(first_node->chars);
			first_node->chars = next;
		}
		free(first_node);
	}
}

static void	ft_remove_quotes(t_char_list *src)
{
	t_char_list	*tmp;
	t_char_list	*tmp2;

	tmp = src;
	tmp2 = NULL;
	src = src->next;
	free(tmp);
	tmp = src;
	while (tmp != NULL)
	{
		if (tmp->next->next == NULL)
		{
			tmp2 = tmp;
			break ;
		}
		tmp = tmp->next;
	}
	tmp = tmp->next;
	free(tmp);
	tmp2->next = NULL;
}

t_argument	*ft_expand_argument(const t_argument_to_expand *argument,
		t_env *env)
{
	t_argument				*argument_with_expanded_vars;
	t_argument				*args_with_expanded_vars;
	t_argument				*splitted_arguments;
	t_argument_to_expand	*tmp_to_expand;
	t_argument				*tmp_split;
	t_argument				*tmp;

	tmp_to_expand = (t_argument_to_expand *)argument;
	args_with_expanded_vars = NULL;
	while (tmp_to_expand != NULL)
	{
		argument_with_expanded_vars = ft_expand_vars_in_argument(
				tmp_to_expand->content, env);
		ft_lstadd_back_argument(&args_with_expanded_vars,
			argument_with_expanded_vars);
		tmp_to_expand = tmp_to_expand->next;
	}
	tmp = args_with_expanded_vars;
	// Si VAR="sdf$x" -> "sdfxxxx" -> "sdf", "xxxx" sont deux args differents
	// Problem "fer" for fir -> while only two times !
	while (tmp != NULL)
		ft_split_argument(&tmp, &splitted_arguments);
	tmp_split = splitted_arguments;
	// Need to be recoded
	while (tmp_split->next != NULL)
	{
		if (tmp_split->chars->value == '\''
			|| tmp_split->chars->value == '"')
			ft_remove_quotes(tmp_split->chars);
		tmp_split = tmp_split->next;
	}
	ft_lstclear_argument(&args_with_expanded_vars);
	return (splitted_arguments);
}

// Modif et supp t_splitted_argument car faut utiliser t_argument
// Chaque arg = un node et chaque node de t_char_list = une lettre