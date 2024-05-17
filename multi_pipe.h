/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipe.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 13:57:47 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/17 15:13:20 by tebandam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MULTI_PIPE_H 

# define MULTI_PIPE_H 

# include "minishell.h"
# include <fcntl.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <unistd.h>

// # ifndef BUFFER_SIZE
// #  define BUFFER_SIZE 1
// # endif

// typedef struct s_vars{
// 	enum{
// 		APPEND,
// 		OUTFILE,
// 		UNASIGN
// 	}	e_last;
// 	pid_t	child;
// 	int		fd_infile;
// 	int		fd_outfile;
// 	int		fd_append;
// 	int		nb_out;
// 	int		nb_cmd;
// 	char	**path;
// 	char	***cmd;
// 	char	**full_cmd;
// 	int		pipe_1[2];
// 	int		tmp_fd;
// 	// A malloc sur nb_hd, la liste chainee sera parcouru lors de l'utilisation des hd
// 	int		nb_hd;
// 	int		hd_w;
// 	int		hd_r;
// }	t_vars;

// /*
// * Exec
// */

// int	check_infile(t_redirection_to_expand *redir);
// int	open_files(t_vars *vars, t_redirection_to_expand *redir);
// char	**find_the_accessible_path(char **path, t_vars *vars, char **command_line);
// void	build_path(char **path, char **bin_path, char **is_valid_cmd, char **full_cmd);
// void	update_full_cmd(char ***full_cmd, char *is_valid_cmd);
// int		fill_command_paths(t_vars *vars, t_argument_to_expand *tmp_arg, t_env *env);
// int		verif_fill_command_paths(t_vars *vars, t_argument_to_expand *tmp_arg, t_env *env);
// void	ft_close_fd(t_vars *vars);
// void	ft_flow_redirection(t_vars *vars, int actual_cmd);
// int	fork_processes(t_vars *vars, char *envp[], t_command_to_expand *tmp);
// int	child_process(t_vars *vars, char *envp[], int actual_cmd);
// void	capture_and_redirection(char *tab, char *tmp, t_vars *vars);
// void	open_fd_infile(t_vars *vars);
// void	open_hd_w(t_vars *vars);
// void	verif_tab(char *tab);
// void	ft_get_nb_hd(t_vars *vars, t_redirection_to_expand *redir);
// void	ft_heredoc(t_vars *vars, t_redirection_to_expand *redir);

// /*
// * GNL
// */

// char	*ft_strjoin_mod(char *s1, char *s2);
// char	*read_loop(char *buf, char *stock, int *len, int fd);
// char	*ft_strchr_rl(const char *s, int c);
// char	*read_line(int fd, char *stock);
// char	*ft_strdup_gnl(const char *s);
// char	*ft_substr_gnl(char const *s, unsigned int start, size_t len);
// char	*extract_line(char *stock);
// char	*ft_strdup(const char *s);
// char	*extract_surplus_line(char *stock);
// char	*get_next_line(int fd);


#endif