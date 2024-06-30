/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:15:04 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/30 11:42:28 by rgobet           ###   ########.fr       */
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

static void	*function_four(const char *argument, int *i, t_char_list **chars)
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

static void	*function_six(const char *argument, int *i,
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

static void	*function_five(const char *argument, int *i,
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
	if (function_six(argument, i, chars) == NULL)
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

static int	exit_code(t_vars *vars, t_char_list **chars, int i)
{
	t_char_list	*tmp;
	char		*exit_code;
	int			s;

	s = 0;
	exit_code = 0;
	exit_code = ft_itoa(vars->exit_code);
	while (exit_code[s])
	{
		tmp = lst_new_char_list();
		if (!tmp)
			return (0);
		tmp->value = exit_code[s];
		tmp->was_in_a_variable = TRUE;
		ft_lstadd_back_char_list(chars, tmp);
		s++;
	}
	free(exit_code);
	i += 2;
	return (i);
}

static int	expand_arg(char *argument, t_env *env, t_char_list **chars, int i)
{
	t_env		*var;
	t_char_list	*tmp;
	char		*var_name;
	int			s;

	s = 0;
	var = NULL;
	var_name = NULL;
	var_name = get_var_name((char *)&argument[i]);
	var = lst_search_env(var_name, env);
	while (var->value[s])
	{
		tmp = lst_new_char_list();
		if (!tmp)
			free(var_name);
		if (!tmp)
			return (0);
		tmp->value = var->value[s];
		tmp->was_in_a_variable = TRUE;
		ft_lstadd_back_char_list(chars, tmp);
		s++;
	}
	i = skip_dolar_var((char *)argument, i);
	free(var_name);
	return (i);
}

static int function_seven(char *argument, t_char_list **chars, int i)
{
	t_char_list	*tmp;

	tmp = NULL;
	tmp = lst_new_char_list();
	if (!tmp)
		return (0);
	tmp->value = argument[i];
	tmp->was_in_a_variable = FALSE;
	ft_lstadd_back_char_list(chars, tmp);
	i = skip_dolar_var((char *)argument, i);
	return (i);
}

// Faire un truc pour verif si tes dans des quotes ou non
// Cette fonction existe dans redirection_expand.c
static t_bool	is_in_quote(char *str, int i)
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

static int	expand_argument(char *argument, t_env *env, t_vars *vars,
	t_char_list **chars)
{
	char		*var_name;
	int			s;
	int			i;

	s = 0;
	i = 0;
	var_name = NULL;
	var_name = get_var_name((char *)&argument[i]);
	if (ft_strcmp(var_name, "$?") == 0
		|| (var_name[0] == '$' && var_name[1] == '?'))
		i = exit_code(vars, chars, i);
	else if (lst_search_env(var_name, env))
		i = expand_arg(argument, env, chars, i);
	else if ((argument[i] == '$' && argument[i + 1] == 0)
		|| (argument[i] == '$' && argument[i + 1] == '"'
			&& is_in_quote(argument, i) == TRUE)
		|| ((argument[i] == '$' && (argument[i + 1] == ' '
					|| argument[i + 1] == '\t' || argument[i + 1] == '\n'))))
		i = function_seven(argument, chars, i);
	else
		i = skip_dolar_var((char *)argument, i);
	if (var_name)
		free(var_name);
	return (i);
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

static t_bool	need_to_increment(char *argument, int i)
{
	t_bool	simple_quote;
	t_bool	last_expand;
	int		j;

	j = 0;
	simple_quote = FALSE;
	while (argument && argument[j] && j < i)
	{
		last_expand = FALSE;
		if (simple_quote == FALSE && argument[j] == '\'')
			simple_quote = TRUE;
		else if (simple_quote == FALSE && argument[j] == '\'')
			simple_quote = FALSE;
		if (argument[j] == '$' && simple_quote == FALSE)
		{
			if (argument[j] == '$' && argument[j] == '?')
			{
				last_expand = FALSE;
				j += 2;
			}
			last_expand = FALSE;
		}
		j++;
	}
	return (TRUE);
}

static t_argument	*ft_expand_vars_in_argument(
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
			function_four(argument, &i, &arg->chars);
		else if (argument[i] == '"'
			|| (is_in_quote((char *)argument, i) == TRUE && argument[i] != '$'))
			function_five(argument, &i,
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

static int	function_verif_quote(t_char_list **tmp_char, char *quote,
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

static void	fill_no_quote_arg(t_char_list **tmp_char,
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

static t_bool	quote_function(t_char_list **tmp_char,
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

static void	rest_argument(t_char_list **tmp_char,
	t_char_list **splitted_chars)
{
	t_bool		in_quote;
	t_char_list	*arg;
	char		quote;

	arg = NULL;
	quote = 0;
	in_quote = FALSE;
	while (*tmp_char && (*tmp_char)->value != ' '
		&& (*tmp_char)->value != '\t' && (*tmp_char)->value != '\n'
		&& in_quote == FALSE)
	{
		while (tmp_char && in_quote == FALSE)
		{
			if (function_verif_quote(tmp_char, &quote, &in_quote) == 1)
				break ;
			fill_no_quote_arg(tmp_char, splitted_chars, quote);
		}
		while (tmp_char && in_quote == TRUE)
		{
			in_quote = quote_function(tmp_char, &quote, in_quote);
			fill_in_quote_arg(tmp_char, splitted_chars, quote);
		}
	}
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

static t_bool	init_function(char *quote, t_argument **tmp,
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

static void	set_var(t_argument *tmp, t_char_list **tmp_char)
{
	*tmp_char = tmp->chars;
	(*tmp_char)->last_pos = FALSE;
}

static int	ft_split_argument(t_argument *argument_to_split,
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

static void	function_one(t_argument_to_expand *tmp_to_expand, t_env *env,
	t_vars *vars, t_argument **args_with_expanded_vars)
{
	t_argument	*argument_with_expanded_vars;

	argument_with_expanded_vars = NULL;
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
		ft_lstadd_back_argument(args_with_expanded_vars,
			argument_with_expanded_vars);
		tmp_to_expand = tmp_to_expand->next;
	}
}

t_argument	*ft_expand_argument(const t_argument_to_expand *argument,
		t_env *env, t_vars *vars)
{
	t_argument				*args_with_expanded_vars;
	t_argument				*splitted_arguments;
	t_argument				*tmp_split;
	t_argument_to_expand	*tmp_to_expand;
	int						tmp;

	tmp = 1;
	tmp_to_expand = (t_argument_to_expand *)argument;
	args_with_expanded_vars = NULL;
	function_one(tmp_to_expand, env, vars, &args_with_expanded_vars);
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
