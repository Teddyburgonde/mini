/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 05:44:21 by tebandam          #+#    #+#             */
/*   Updated: 2024/06/30 15:36:22 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	hide_and_update_env_var(t_env *tmp_env, char *cmd)
{
	if (tmp_env->value)
		free(tmp_env->value);
	tmp_env->hide = TRUE;
	if (tmp_env->full_path)
		free(tmp_env->full_path);
	tmp_env->full_path = copy(cmd);
	tmp_env->value = NULL;
}

void	update_env_var(t_env *tmp_env, char *cmd, char *value)
{
	free(tmp_env->value);
	if (value == NULL || value[0] == 0)
		tmp_env->hide = TRUE;
	else
		tmp_env->hide = FALSE;
	if (tmp_env->value)
		free(tmp_env->value);
	tmp_env->value = copy(value);
	free(tmp_env->full_path);
	tmp_env->full_path = copy(cmd);
}


int	verif_export(char *str)
{
	int		i;
	int		len_mid;
	t_bool	append;

	i = 0;
	append = TRUE;
	if (str[0] == '_' && str[1] == '\0')
		return (0);
	if (ft_isdigit(str[0]) || str[0] == '=')
	{
		ft_putstr_fd("minishell: export: `", 2);
		write (2, str, ft_strlen(str));
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	len_mid = ft_strcspn(str, "=");
	if (str[len_mid - 1] == '+')
		len_mid--;
	else
		append = FALSE;
	while (str[i])
	{
		if (str[i] < 48
			|| (str[i] > 57 && str[i] < 65)
			|| (str[i] > 90 && str[i] < 97)
			|| str[i] > 122)
		{
			if ((i != len_mid && append == FALSE)
				|| (i != len_mid + 1 && append == TRUE)
				|| (i != len_mid && append == TRUE))
			{
				if (append == FALSE && str[i] != '_'
					&& i <= len_mid)
				{
					ft_putstr_fd(
						" not a valid identifier\n", 2);
					return (1);
				}
				else if (append == TRUE && str[i] != '_'
					&& i < len_mid)
				{
					ft_putstr_fd(
						"minishell: syntax error near unexpected token `", 2);
					write(2, &str[i], 1);
					write(2, "'\n", 2);
					return (1);
				}
			}
		}
		i++;
	}
	if ((len_mid == ft_strlen(str) && append == FALSE)
		|| (len_mid + 1 == ft_strlen(str) && append == TRUE))
		return (2);
	return (0);
}
