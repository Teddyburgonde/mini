/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipe.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tebandam <tebandam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 13:57:47 by tebandam          #+#    #+#             */
/*   Updated: 2024/05/06 10:48:29 by tebandam         ###   ########.fr       */
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

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef struct s_vars{
	enum{
		APPEND,
		OUTFILE,
		UNASIGN
	}	e_last;
	pid_t	child;
	int		fd_infile;
	int		fd_outfile;
	int		fd_append;
	int		nb_out;
	int		nb_cmd;
	char	**path;
	char	***cmd;
	char	**full_cmd;
	int		pipe_1[2];
	int		tmp_fd;
	// A malloc sur nb_hd, la liste chainee sera parcouru lors de l'utilisation des hd
	int		nb_hd;
	int		hd_w;
	int		hd_r;
}	t_vars;

#endif