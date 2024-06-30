/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_expand_argument.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:23:23 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/30 15:40:57 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

static int	invalid_var_name(char *argument, t_char_list **chars, int i)
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

int	expand_argument(char *argument, t_env *env, t_vars *vars,
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
		i = invalid_var_name(argument, chars, i);
	else
		i = skip_dolar_var((char *)argument, i);
	if (var_name)
		free(var_name);
	return (i);
}

t_bool	need_to_increment(char *argument, int i)
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
