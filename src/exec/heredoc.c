/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:11:14 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/21 13:48:04 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_bool	need_to_be_expand(char *str, t_env *env)
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

// Malloc a faire et a verif les index.
static int	ft_strlen_with_expand(char *str, t_env *env, t_vars *vars)
{
	int		i;
	int		j;
	int		count;
	char	*var_name;
	char	*exit_code;
	t_env	*var;

	i = 0;
	j = 0;
	count = 0;
	while (str && str[i])
	{
		count += j;
		j = 0;
		if (str[i] == '$')
		{
			var_name = get_var_name(&str[i]);
			if (var_name)
			{
				var = lst_search_env(var_name, env);
				if (var)
				{
					while (var->value && var->value[j])
						j++;
				}
			}
			if (var_name == NULL || var_name[0] == 0)
			{
				j = i;
				i = skip_dolar_var(str, i);
				count -= j - i;
			}
			else if (var_name[0] == '$' && var_name[1] == '?')
			{
				exit_code = ft_itoa(vars->exit_code);
				while (exit_code && exit_code[j])
					j++;
				i += 2;
				free(exit_code);
			}
			else
				i = skip_dolar_var(str, i);
			free(var_name);
		}
		if (str[i] && str[i] != '$')
		{
			i++;
			count++;
		}
	}
	if (j != 0)
		return (count + j);
	return (count);
}

static char	*expand_line(char *str, t_env *env, t_vars *vars)
{
	int		i;
	int		t;
	int		j;
	int		size;
	char	*tmp;
	char	*var_name;
	char	*exit_code;
	t_env	*var;

	i = 0;
	t = 0;
	j = 0;
	size = ft_strlen_with_expand(str, env, vars) + 1;
	tmp = copy(str);
	free(str);
	str = malloc(sizeof(char) * size);
	if (!str)
	{
		str = NULL;
		return (str);
	}
	while (tmp && tmp[t]) // Verif $?$USER
	{
		i += j;
		j = 0;
		if (tmp[t] == '$')
		{
			var_name = get_var_name(&tmp[t]);
			if (var_name == NULL || var_name[0] == 0)
			{
				t = skip_dolar_var(tmp, t);
				continue ;
			}
			else if (tmp[t] == '$' && (tmp[t + 1] == ' '
					|| tmp[t + 1] == '\t' || tmp[t + 1] == '\n'))
				str[i + j] = tmp[t];
			else if (var_name[0] == '$' && var_name[1] == '?')
			{
				exit_code = ft_itoa(vars->exit_code);
				while (exit_code && exit_code[j])
				{
					str[i + j] = exit_code[j];
					j++;
				}
				free(exit_code);
			}
			else
			{
				var = lst_search_env(var_name, env);
				if (var)
				{
					while (var->value[j])
					{
						str[i + j] = var->value[j];
						j++;
					}
				}
			}
			if (tmp[t] == '$' && tmp[t + 1] == '?')
				t += 2;
			else
				t = skip_dolar_var(tmp, t) - 1;
			free(var_name);
			j--;
		}
		else
			str[i] = tmp[t];
		if (tmp[t] && tmp[t] != '$')
			t++;
		i++;
	}
	str[size - 1] = 0;
	free(tmp);
	return (str);
}

static void	fill_tmp_content(char **tmp_content, t_redirection *redirection,
	int fd_tmp, t_env *env, t_vars *vars)
{
	t_redirection	*tmp_redirection;

	tmp_redirection = redirection;
	*tmp_content = readline("> ");
	if (*tmp_content == NULL)
	{
		free(*tmp_content);
		ft_putstr_fd("bash: warning: here-document at line 5 delimited by end-of-file (wanted `:')", 2);
		return ;
	}
	if (ft_strcmp(*tmp_content, tmp_redirection->limiter) == 0)
	{
		free(*tmp_content);
		*tmp_content = NULL;
		//exit (500);
		// close(fd_tmp);
		return ;
	}
	else
	{
		if (need_to_be_expand(*tmp_content, env) >= TRUE)
			*tmp_content = expand_line(*tmp_content, env, vars);
		ft_putstr_fd(*tmp_content, fd_tmp);
		ft_putstr_fd("\n", fd_tmp);
	}
}

void	ft_heredoc(t_redirection *redirection,
		t_redirection_to_expand *all, t_bool save,
			t_env *env, t_vars *vars)
{
	char			*tmp_content;
	int				count;

	tmp_content = NULL;
	while (all && save == TRUE)
	{
		if (all->e_type == REDIRECTION_HEREDOC)
		{
			count = 0;
			while (ft_strcmp(tmp_content, all->arg) != 0 || count == 0)
			{
				if (redirection->e_position == HERE && save == TRUE && is_last(all) == all)
				{
					// open_fd_tmp_for_heredoc(&fd_tmp);
					// if (fd_tmp < 0)
					// 	return ;
					while (1)
					{
						fill_tmp_content(&tmp_content, redirection, redirection->infile_fd, env, vars);
						if (tmp_content == NULL)
							return ;
						else
							free(tmp_content);
					}
					// close(fd_tmp);
				}
				else
				{
					if (tmp_content)
						free(tmp_content);
					tmp_content = NULL;
					tmp_content = readline("> ");
					if (tmp_content == NULL)
					{
						free(tmp_content);
						ft_putstr_fd("bash: warning: here-document at line 5 delimited by end-of-file (wanted `:')", 2);
						break ;
					}
				}
				count++;
			}
			free(tmp_content);
			tmp_content = NULL;
		}
		all = all->next;
	}
}

