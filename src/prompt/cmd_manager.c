/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 15:10:36 by rgobet            #+#    #+#             */
/*   Updated: 2024/05/14 15:42:26 by tebandam         ###   ########.fr       */
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























int	check_infile(t_redirection_to_expand *redir)
{
	t_redirection_to_expand	*tmp;

	tmp = redir;
	while (tmp)
	{
		if (tmp->e_type == REDIRECTION_INFILE)
		{
			if (access(tmp->arg, F_OK) != 0 || access(tmp->arg, R_OK) != 0
				|| access(tmp->arg, W_OK) != 0)
			{
				ft_putstr_fd("Error\nPermission denied\n", 2);
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}

int	open_files(t_vars *vars, t_redirection_to_expand *redir)
{
	t_redirection_to_expand	*tmp;
	t_bool					already_get;
	int						hd;
	int						outf;
	int						inf;
	int						app;

	hd = 0;
	outf = 0;
	inf = 0;
	app = 0;
	vars->fd_append = 0;
	vars->fd_infile = 0;
	vars->fd_outfile = 0;
	vars->hd_r = 0;
	vars->hd_w = 0;
	vars->e_last = UNASIGN;
	already_get = FALSE;
	tmp = redir;
	while (tmp)
	{
		if (tmp->e_type == REDIRECTION_HEREDOC)
			hd++;
		else if (tmp->e_type == REDIRECTION_OUTFILE)
		{
			outf++;
			vars->nb_out++;
		}
		else if (tmp->e_type == REDIRECTION_INFILE)
			inf++;
		else if (tmp->e_type == REDIRECTION_APPEND)
			app++;
		tmp = tmp->next;
	}
	tmp = redir;
	// Droit a verif
	while (tmp)
	{
		if (tmp->e_type == REDIRECTION_HEREDOC)
		{
			if (hd == 1)
			{
				vars->hd_r = open(tmp->arg, O_RDONLY, 0644);
				vars->hd_w = open(tmp->arg, O_WRONLY, 0644);
			}
			hd--;
		}
		else if (tmp->e_type == REDIRECTION_OUTFILE)
		{
			if (outf == 1)
				vars->fd_outfile = open(tmp->arg, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			else
			{
				vars->tmp_fd = open(tmp->arg, O_CREAT, 0644);
				close(vars->tmp_fd);
				vars->nb_out++;
			}
			vars->e_last = OUTFILE;
			outf--;
		}
		else if (tmp->e_type == REDIRECTION_INFILE)
		{
			if (already_get == FALSE)
				vars->fd_infile = open(tmp->arg, O_RDONLY, 0644);
			inf--;
		}
		else if (tmp->e_type == REDIRECTION_APPEND)
		{
			if (app == 1)
				vars->fd_append = open(tmp->arg, O_WRONLY | O_CREAT, 0644);
			else
			{
				// A verif
				vars->tmp_fd = open(tmp->arg, O_CREAT, 0644);
				close(vars->tmp_fd);
			}
			vars->e_last = APPEND;
			app--;
		}
		tmp = tmp->next;
	}
	return (0);
}

char	**grep_path(t_env **env)
{
	t_env	*tmp_env;
	char	**tmp;

	tmp_env = *env;
	while (tmp_env != NULL)
	{
		if (ft_strcmp("PATH", tmp_env->var_name) == 0)
		{
			tmp = ft_split(tmp_env->var, ':');
			return (tmp);
		}
		tmp_env = tmp_env->next;
	}
	return (NULL);
}

static int	ft_lstsize_command(t_command_to_expand *cmd)
{
	int	counter;

	counter = 0;
	while (cmd != NULL)
	{
		cmd = cmd->next;
		counter++;
	}
	return (counter);
}
// ici j'ai commencé a rajouter

void	ft_free_tab_3d(t_vars *vars)
{
	int	i;
	int	j;

	i = 0;
	if (!vars->cmd)
		return ;
	while (vars->cmd[i])
	{
		j = 0;
		while (vars->cmd[i][j])
		{
			free(vars->cmd[i][j]);
			j++;
		}
		free(vars->cmd[i]);
		i++;
	}
	free(vars->cmd);
}


size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	j = ft_strlen(src);
	i = 0;
	if (size != 0)
	{
		while (src[i] && i < (size - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (j);
}

void	update_full_cmd(char ***full_cmd, char *is_valid_cmd)
{
	free((*full_cmd)[0]);
	(*full_cmd)[0] = NULL;
	(*full_cmd)[0] = malloc(ft_strlen(is_valid_cmd) + 1);
	ft_strlcpy((*full_cmd)[0], is_valid_cmd, ft_strlen(is_valid_cmd) + 1);
	free(is_valid_cmd);
}

void	build_path(char **path, char **bin_path,
	char **is_valid_cmd, char **full_cmd)
{
	int	i;

	i = 0;
	while (path[i])
	{
		*bin_path = ft_strjoin(path[i++], "/");
		*is_valid_cmd = ft_strjoin(*bin_path, full_cmd[0]);
		free(*bin_path);
		if (access(*is_valid_cmd, X_OK) == 0)
		{
			update_full_cmd(&full_cmd, *is_valid_cmd);
			break ;
		}
		free(*is_valid_cmd);
	}
}

char	**find_the_accessible_path(char **path, t_vars *vars, char **command_line)
{
	int		i;
	char	*bin_path;
	char	*is_valid_cmd;

	i = 0;	
	vars->cmd[i] = command_line;
	if (vars->full_cmd == NULL || vars->full_cmd[0] == NULL
		|| vars->full_cmd[0][0] == '\0')
	{
		ft_putstr_fd("Error\nCmd invalid", 2);
		ft_free_tab_3d(vars);
		ft_free(vars->path);
		ft_free(vars->full_cmd);
		close(vars->fd_infile);
		close(vars->fd_outfile);
		exit(1);
	}
	if (access(vars->full_cmd[0], X_OK) == 0)
		return (vars->full_cmd);
	build_path(path, &bin_path, &is_valid_cmd, vars->full_cmd);
	return (vars->full_cmd);
}

int	fill_command_paths(t_vars *vars, t_argument_to_expand *tmp_arg, t_env *env)
{
	t_argument	*final_parsing;
	char		**command_line;
	int			i;

	i = 0;
	while (i < vars->nb_cmd)
	{
		final_parsing = ft_expand_argument(tmp_arg, env);
		command_line = ft_setup_command(final_parsing);
		vars->full_cmd = command_line;
		vars->cmd[i] = find_the_accessible_path(vars->path, vars, command_line);
		if (vars->cmd[i] == NULL)
		{
			return (-1);
		}
		tmp_arg = tmp_arg->next;
		if (final_parsing)
			ft_lstclear_argument(&final_parsing);
		i++;
	}
	vars->cmd[i] = NULL;
	return (0);
}

int	verif_fill_command_paths(t_vars *vars, t_argument_to_expand *tmp_arg, t_env *env)
{
	if (fill_command_paths(vars, tmp_arg, env) == -1)
	{
		close(vars->fd_infile);
		close(vars->fd_outfile);
		ft_free(vars->path);
		ft_free_tab_3d(vars);
		return (1);
	}
	return (0);
}

void	ft_close_fd(t_vars *vars)
{
	close(vars->tmp_fd);
	close(vars->fd_infile);
	close(vars->fd_outfile);
	close(vars->fd_append);
	close(vars->hd_r);
	close(vars->hd_w);
}

void	ft_flow_redirection(t_vars *vars, int actual_cmd)
{
	if (actual_cmd == 0)
	{
		// DONE
		if (dup2(vars->fd_infile, STDIN_FILENO) < 0)
			perror("dup2");
		if (dup2(vars->pipe_1[1], STDOUT_FILENO) < 0)
			perror("dup2");
	}
	else if (actual_cmd == vars->nb_cmd - 1)
	{
		// DONE
		if (vars->e_last == OUTFILE)
		{	
			if (dup2(vars->tmp_fd, STDIN_FILENO) < 0)
				perror("dup2");
			if (dup2(vars->fd_outfile, STDOUT_FILENO) < 0)
				perror("dup2");
		}
		else if (vars->e_last == APPEND)
		{
			if (dup2(vars->tmp_fd, STDIN_FILENO) < 0)
				perror("dup2");
			if (dup2(vars->fd_append, STDOUT_FILENO) < 0)
				perror("dup2");
		}
	}
	else
	{
		// ???
		if (dup2(vars->tmp_fd, STDIN_FILENO) < 0)
			perror("dup2");
		if (dup2(vars->pipe_1[1], STDOUT_FILENO) < 0)
			perror("dup2");
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 08:04:31 by tebandam          #+#    #+#             */
/*   Updated: 2023/11/08 13:04:57 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


static size_t	search_leng(int n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		len++;
	while (n)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static void	fill_str(char *str, int n, size_t len)
{
	size_t	i;

	str[len] = '\0';
	i = 0;
	if (n < 0)
	{
		str[0] = '-';
		i = 1;
	}
	while (len > i)
	{
		len--;
		if (n < 0)
		{
			str[len] = n % 10 * (-1) + '0';
			n = n / 10;
		}
		else
		{
			str[len] = n % 10 + '0';
			n = n / 10;
		}
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	len;

	len = search_leng(n);
	str = malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	fill_str(str, n, len);
	return (str);
}
// a supp au dessus 

int	child_process(t_vars *vars, char *envp[], int actual_cmd)
{
	ft_flow_redirection(vars, actual_cmd);
	if (vars->tmp_fd != -1)
		close(vars->tmp_fd);
	close(vars->pipe_1[0]);
	close(vars->pipe_1[1]);
	close(vars->fd_infile);
	close(vars->fd_outfile);
	//printf("JE PASSE ICI\n");
	ft_putstr_fd("TEST\n",  2);
	for(int i = 0; envp[i] != NULL; i++ ){
		char* nom = ft_itoa(i);
		ft_putstr_fd("i = ", 2);
		ft_putstr_fd(nom, 2);
		ft_putstr_fd(" ", 2);
		ft_putstr_fd(envp[i], 2);
		ft_putstr_fd("\n", 2);
	}
	execve(vars->cmd[actual_cmd][0], vars->cmd[actual_cmd], envp);
	perror("Execve");
	ft_free(vars->path);
	ft_free_tab_3d(vars);
	return (1);
}

int	fork_processes(t_vars *vars, char *envp[], t_command_to_expand *tmp)
{
	int	i;

	i = 0;
	ft_heredoc(vars, tmp->redirections);
	while (i < vars->nb_cmd)
	{
		open_files(vars, tmp->redirections);
		if (pipe(vars->pipe_1) == -1)
			return (EXIT_FAILURE);
		vars->child = fork();
		if (vars->child == 0)
			child_process(vars, envp, i);
		else if (vars->child < 0)
		{
			perror("fork");
			return (EXIT_FAILURE);
		}
		else
		{
			if (vars->tmp_fd != -1)
				close(vars->tmp_fd);
			vars->tmp_fd = dup(vars->pipe_1[0]);
			close(vars->pipe_1[0]);
			close(vars->pipe_1[1]);
		}
		tmp = tmp->next;
		i++;
	}
	return (0);
}

int	ft_lstsize_env(t_env *env)
{
	int	counter;

	counter = 0;
	while (env != NULL)
	{
		env = env->next;
		counter++;
	}
	return (counter);
}

char	**env_to_char(t_env *env)
{
	char	**tmp;
	int		i;

	tmp = malloc((ft_lstsize_env(env) + 1) * sizeof(char *));
	if (!tmp)
		return (NULL);
	i = 0;
	while (env)
	{
		tmp[i] = env->full_path;
		env = env->next;
		i++;
	}
	tmp[i] = 0;
	return (tmp);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return (((unsigned char)s1[i] - (unsigned char)s2[i]));
		i++;
	}
	return (0);
}

void	capture_and_redirection(char *tab, char *tmp, t_vars *vars)
{
	while (ft_strncmp(tab, tmp, ft_strlen(tmp)))
	{
		ft_putstr_fd(tab, vars->hd_w);
		free(tab);
		write(1, "> ", 2);
		tab = get_next_line(STDIN_FILENO);
		if (!tab)
		{
			ft_putstr_fd("Error\n No limiter for heredoc", 2);
			exit(1);
		}
	}
	free(tab);
	free(tmp);
	close(vars->hd_w);
}

void	open_fd_infile(t_vars *vars)
{
	vars->fd_infile = open("TMP_HEREDOC", O_RDONLY, 0644);
	if (!vars->fd_infile)
	{
		perror("Error opening files");
		exit(1);
	}
	unlink("TMP_HEREDOC");
}

void	open_hd_w(t_vars *vars)
{
	vars->hd_w = open("TMP_HEREDOC", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (vars->hd_w == -1)
	{
		perror("Error opening files");
		exit(1);
	}
}

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
		vars.path = grep_path(env);
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
