/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:15:04 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/30 15:27:04 by tebandam         ###   ########.fr       */
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

static void	ft_expand_arguments(t_argument_to_expand *tmp_to_expand, t_env *env,
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
	ft_expand_arguments(tmp_to_expand, env, vars, &args_with_expanded_vars);
	splitted_arguments = NULL;
	if (args_with_expanded_vars != NULL)
	{
		while (tmp != 0)
			tmp = ft_split_argument(args_with_expanded_vars,
					&splitted_arguments);
		tmp_split = splitted_arguments;
	}
	if (args_with_expanded_vars)
		ft_lstclear_argument(&args_with_expanded_vars);
	else
		return (NULL);
	return (splitted_arguments);
}
