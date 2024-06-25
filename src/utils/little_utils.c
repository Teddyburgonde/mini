/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   little_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 15:27:46 by rgobet            #+#    #+#             */
/*   Updated: 2024/06/25 15:32:29 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_bool	need_to_be_expand_hd(char *str, t_env *env)
{
	int		i;
	char	*var_name;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$')
		{
			var_name = get_var_name(&str[i]);
			if (lst_search_env(var_name, env) || ft_strcmp(var_name, "$?") == 0
				|| (var_name[0] == '$' && var_name[1] == '?'))
			{
				free (var_name);
				return (TRUE);
			}
			else
			{
				free (var_name);
				return (2);
			}
		}
		i++;
	}
	return (FALSE);
}

void	refresh_index(char *str, int *i, int *count, int j)
{
	if (str[*i] && str[*i] != '$')
	{
		*i += 1;
		*count += 1;
	}
	*count += j;
}

void	init_expand(int *i, int *j, int *t, char **var_name)
{
	*i = 0;
	*j = 0;
	*t = 0;
	*var_name = NULL;
}

void	increment_index(int *i, int *t, char *tmp)
{
	if (tmp[*t] && tmp[*t] != '$')
		*t += 1;
	*i += 1;
}
