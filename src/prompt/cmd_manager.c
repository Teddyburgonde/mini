/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 15:10:36 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/15 14:49:55 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_heredoc(t_vars *vars, t_redirection_to_expand *redir);

// static char	*access_to_cmd(char *cmd, char **path, char *error)
// {
// 	int		i;
// 	char	*tmp;

// 	i = 0;
// 	while (path[i])
// 	{
// 		tmp = ft_strjoin(path[i], cmd[0]);
// 		if (access(tmp, X_OK) == 0)
// 		{
// 			ft_free(path);
// 			free(cmd[0]);
// 			return (tmp);
// 		}
// 		free(tmp);
// 		i++;
// 	}
// 	write(2, "command not found: ", 19);
// 	write(2, error, ft_strlen(error));
// 	write(2, "\n", 1);
// 	return (NULL);
// }

// static char	*verification(t_vars *vars, char **cmd, char **env)
// {
// 	int		i;
// 	char	*tmp;
// 	char	*error;
// 	char	**path;

// 	i = 0;
// 	path = NULL;
// 	while (env[i])
// 	{
// 		if (env[i][0] == 'P' && env[i][1] == 'A' && env[i][2] == 'T'
// 		&& env[i][3] == 'H' && env[i][4] == '=')
// 		{
// 			path = ft_split(&env[i][5], ':');
// 			break ;
// 		}
// 		i++;
// 	}
// 	if (!path)
// 		return (2);
// 	error = copy(cmd[0]);
// 	cmd[0] = slash_add("/", cmd[0]);
// 	tmp = access_to_cmd(cmd[0], path, error);
// 	if (tmp == NULL)
// 		return (NULL);
// 	return (tmp);
// }

// static void	single_cmd(t_vars *vars, char **env)
// {
// 	pid_t	pid;
// 	char	**cmd;

// 	cmd = ft_split(vars->input, ' ');
// 	cmd[0] = verification(vars, cmd, env);
// 	if (cmd[0])
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			ft_free(cmd);
// 			return (1);
// 		}
// 		if (pid == 0)
// 		{
// 			execve(cmd[0], cmd, env);
// 			ft_free(cmd);
// 		}
// 		else
// 		{
// 			waitpid(pid, 0, 2);
// 			ft_free(cmd);
// 			return ;
// 		}
// 	}
// }









// static int	cmd_selector(t_env **env, char **command_line)
// {
// 	t_env	*tmp;

// 	tmp = *env;
// 	if (ft_strcmp(command_line[0], "echo") == 0)
// 	{
// 		ft_echo(command_line);
// 		return (0);
// 	}
// 	else if (ft_strcmp(command_line[0], "pwd") == 0)
// 	{
// 		ft_pwd();
// 		return (0);
// 	}
// 	else if (ft_strcmp(command_line[0], "unset") == 0)
// 	{
// 		unset(env, command_line[1]);
// 		return (0);
// 	}
// 	else if (ft_strcmp(command_line[0], "export") == 0)
// 	{
// 		export(env, command_line);
// 		return (0);
// 	}
// 	else if (ft_strcmp(command_line[0], "printenv") == 0
// 		|| ft_strcmp(command_line[0], "env") == 0)
// 	{
// 		if (ft_strcmp(command_line[0], "env") == 0 && command_line[1] != NULL
// 			&& access(command_line[1], F_OK) == 0)
// 		{
// 			ft_putstr_fd("env: ‘", 2);
// 			ft_putstr_fd(command_line[1], 2);
// 			ft_putstr_fd("‘: No such file or directory", 2);
// 		}
// 		else if (ft_strcmp(command_line[0], "env") == 0
// 			&& command_line[1] == NULL)
// 		{
// 			while (tmp)
// 			{
// 				printf("%s\n", tmp->full_path);
// 				tmp = tmp->next;
// 			}
// 		}
// 		return (0);
// 	}
// 	else if (ft_strcmp(command_line[0], "exit") == 0)
// 		return (atoi(command_line[1]));
// 	return (666);
// }

// ici placer gnl demain 05/05/2024
char	*ft_strjoin_mod(char *s1, char *s2)
{
	char	*tab;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j = 0;
	tab = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!tab)
		return (NULL);
	while (s1[i])
	{
		tab[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		tab[i + j] = s2[j];
		j++;
	}
	tab[i + j] = '\0';
	free(s1);
	s1 = NULL;
	return (tab);
}

static char	*read_loop(char *buf, char *stock, int *len, int fd)
{
	buf = malloc(sizeof(char) * BUFFER_SIZE + 1);
	*len = read(fd, buf, BUFFER_SIZE);
	if (*len == -1 || (*len == 0 && stock[0] == '\0'))
	{
		free(buf);
		free(stock);
		return (NULL);
	}
	buf[*len] = 0;
	stock = ft_strjoin_mod(stock, buf);
	free(buf);
	buf = NULL;
	return (stock);
}

char	*ft_strchr_rl(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)(&s[i]));
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)(&s[i]));
	return (NULL);
}

static char	*read_line(int fd, char *stock)
{
	char	*buf;
	int		len;

	len = 1;
	buf = NULL;
	if (stock != NULL)
	{
		buf = malloc(sizeof(char) * BUFFER_SIZE + 1);
		len = read(fd, buf, BUFFER_SIZE);
		buf[len] = 0;
		if (len > 0)
			stock = ft_strjoin_mod(stock, buf);
		free(buf);
		buf = NULL;
	}
	else
	{
		stock = malloc(sizeof(char));
		stock[0] = 0;
	}
	while (len > 0 && ft_strchr_rl(stock, '\n') == NULL)
		stock = read_loop(buf, stock, &len, fd);
	free(buf);
	return (stock);
}

char	*ft_strdup_gnl(const char *s)
{
	char		*tab;
	int			i;

	i = 0;
	tab = malloc(sizeof(char) * ft_strlen(s) + 1);
	if (tab)
	{
		while (s[i])
		{
			tab[i] = s[i];
			i++;
		}
		tab[i] = '\0';
	}
	return ((char *)tab);
}

char	*ft_substr_gnl(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*tab;

	if (!s)
		return (NULL);
	if ((unsigned int)ft_strlen(s) < start)
		return (ft_strdup_gnl(""));
	j = ft_strlen(s + start);
	if (j < len)
		len = j;
	tab = malloc(sizeof(char) * (len + 1));
	if (!tab)
		return (NULL);
	i = 0;
	while (i < len)
	{
		tab[i] = s[start + i];
		i++;
	}
	tab[i] = '\0';
	return (tab);
}

static char	*extract_line(char *stock)
{
	int		i;
	char	*line;

	line = NULL;
	if (stock == NULL)
		return (NULL);
	i = 0;
	while (stock[i] && stock[i] != '\n')
		i++;
	if (stock[i] == '\n')
		i++;
	line = ft_substr_gnl(stock, 0, i);
	return (line);
}

char	*ft_strdup(const char *s)
{
	char		*tab;
	int			i;

	i = 0;
	tab = malloc(sizeof(char) * ft_strlen(s) + 1);
	if (tab)
	{
		while (s[i])
		{
			tab[i] = s[i];
			i++;
		}
		tab[i] = '\0';
	}
	return ((char *)tab);
}


static char	*extract_surplus_line(char *stock)
{
	int		i;
	int		j;
	char	*new_stock;

	if (stock == NULL)
		return (NULL);
	i = 0;
	while (stock[i] && stock[i] != '\n')
		i++;
	if (stock[i] == '\n')
		i++;
	j = ft_strlen(stock);
	if (i == j)
	{
		free(stock);
		return (NULL);
	}
	new_stock = ft_substr_gnl(stock, i, j);
	free(stock);
	return (new_stock);
}

char	*get_next_line(int fd)
{
	static char	*stock = NULL;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stock = read_line(fd, stock);
	line = extract_line(stock);
	stock = extract_surplus_line(stock);
	return (line);
}

void	verif_tab(char *tab)
{
	if (!tab)
	{
		ft_putstr_fd("Error\n No limiter for heredoc", 2);
		exit(1);
	}
}

void	ft_get_nb_hd(t_vars *vars, t_redirection_to_expand *redir)
{
	int	count;

	count = 0;
	while (redir)
	{
		if (redir->e_type == REDIRECTION_HEREDOC)
			count++;
		redir = redir->next;
	}
	vars->nb_hd = count;
}

void	ft_heredoc(t_vars *vars, t_redirection_to_expand *redir)
{
	char	*tab;
	int		i;

	i = 0;
	tab = NULL;
	ft_get_nb_hd(vars, redir);
	open_hd_w(vars);
	while (redir && vars->nb_hd > 0)
	{
		while (redir->e_type != REDIRECTION_HEREDOC)
			redir = redir->next;
		write(1, "> ", 2);
		tab = get_next_line(STDIN_FILENO);
		verif_tab(tab);
		capture_and_redirection(tab, redir->arg, vars);
		redir = redir->next;
		vars->nb_hd--;
	}
	close(vars->hd_w);
	open_fd_infile(vars);
}

int	ft_cmd_manager(t_env **env, t_command_line_parsing_result *cmd)
{
	t_vars							vars;
	t_command_to_expand				*tmp;
	t_redirection_to_expand			*tmp_redir;
	t_argument_to_expand			*tmp_arg;
	char							**command_line;
	char							**envp;
	int								exit;
	int								i;

	tmp = cmd->commands;
	tmp_redir = tmp->redirections;
	tmp_arg = tmp->arguments;
	exit = 666;
	command_line = NULL;
	// exit = cmd_selector(env, command_line);
	if (exit == 666 && env)
	{
		i = 0;
		ft_expand_redirections(tmp_redir, *env);
		check_infile(tmp->redirections);
		vars.nb_cmd = ft_lstsize_command(cmd->commands);
		vars.path = ft_split(lst_search_env("PATH", *env), ':');
		vars.cmd = ft_calloc(vars.nb_cmd + 1, sizeof(char **));
		// Mettre les built-ins V
		verif_fill_command_paths(&vars, tmp_arg, *env);
		vars.tmp_fd = -1;
		envp = env_to_char(*env);
		// fork_processes(&vars, envp, tmp);
		ft_free(vars.path);
		ft_free_tab_3d(&vars);
		ft_close_fd(&vars);
		free(envp);
		while (waitpid(-1, NULL, 0) != -1)
			continue ;
	}
	else if (exit != 0)
		write(2, "Error !\n", 8);
	return (0);
}
