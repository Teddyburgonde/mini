/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgobet <rgobet@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:01:08 by rgobet            #+#    #+#             */
/*   Updated: 2024/03/18 11:35:06 by rgobet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_env	*ft_lstnew_env(void)
{
	t_env	*list;

	list = malloc(sizeof(t_env));
	if (!list)
	{
		write (2, "Initialize environment variable has an error.\n", 46);
		exit(1);
	}
	list->next = NULL;
	return (list);
}

void	ft_lstclear_env(t_env **lst)
{
	t_env	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		free(tmp->full_path);
		free(tmp->var_name);
		free(tmp->var);
		free(tmp);
	}
}

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*tmp;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp && tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
}

t_env	*lst_search_env(char *s, t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->var_name, &s[1]) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}
