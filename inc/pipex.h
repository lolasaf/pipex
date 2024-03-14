/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-safa <wel-safa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 18:19:13 by wel-safa          #+#    #+#             */
/*   Updated: 2024/03/14 16:00:24 by wel-safa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include "libft.h"

typedef struct s_pipex
{
	int		infile;
	int		outfile;
	char	*cmd1;
	char	*cmd2;
	char	**cmd_av_1;
	char	**cmd_av_2;
	int		pipefd[2];
	pid_t	cpid1;
	pid_t	cpid2;
	int		status1;
	int		status2;
}			t_pipex;

void	ft_pipex(char **argv, char **envp, t_pipex *pipex);
char	*ft_execpath(t_pipex *pipex, char *exec, char **envp);
void	ft_free_splits(char **splits);
void	free_close(t_pipex *pipex, int free_a, int close_f, int close_p);