/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-safa <wel-safa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 23:59:57 by wel-safa          #+#    #+#             */
/*   Updated: 2024/03/07 00:54:27 by wel-safa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*Child process 1*/
void	ft_fork_1(t_pipex *pipex, char **envp)
{
	pipex->cpid1 = fork();
	if (pipex->cpid1 < 0)
	{
		perror("Fork error\n");
		free_close(pipex, 1, 1, 1);
		exit(EXIT_FAILURE);
	}
	if (pipex->cpid1 == 0)
	{
		dup2(pipex->infile, STDIN_FILENO);
		dup2(pipex->pipefd[1], STDOUT_FILENO);
		free_close(pipex, 0, 1, 1);
		execve(pipex->cmd1, pipex->cmd_av_1, envp);
		perror("execve\n");
		free_close(pipex, 1, 0, 0);
		exit(EXIT_FAILURE);
	}
}

/*Child process 2*/
void	ft_fork_2(t_pipex *pipex, char **envp)
{
	pipex->cpid2 = fork();
	if (pipex->cpid2 < 0)
	{
		perror("Fork error\n");
		free_close(pipex, 1, 1, 1);
		exit(EXIT_FAILURE);
	}
	if (pipex->cpid2 == 0)
	{
		dup2(pipex->pipefd[0], STDIN_FILENO);
		dup2(pipex->outfile, STDOUT_FILENO);
		free_close(pipex, 0, 1, 1);
		execve(pipex->cmd2, pipex->cmd_av_2, envp);
		perror("execve\n");
		free_close(pipex, 1, 0, 0);
		exit(EXIT_FAILURE);
	}
}

/*ft_pipex splits commands inputs to command and options
it calls ft_execpath to find the paths of the two executable commands
ft_execpath returns allocated memory containing the full path of each command
pipex_cp function is then called to execute the pipe and the processes
*/
void	ft_pipex(char **argv, char **envp, t_pipex *pipex)
{
	pipex->cmd_av_1 = ft_split(argv[2], ' ');
	pipex->cmd_av_2 = ft_split(argv[3], ' ');
	pipex->cmd1 = ft_execpath(pipex->cmd_av_1[0], envp);
	pipex->cmd2 = ft_execpath(pipex->cmd_av_2[0], envp);
	if (pipex->cmd1 == NULL || pipex->cmd2 == NULL)
	{
		perror("one or two executable files were not found\n");
		free_close(pipex, 1, 1, 0);
		exit(EXIT_FAILURE);
	}
	if (pipe(pipex->pipefd) < 0)
	{
		perror("Pipe error\n");
		free_close(pipex, 1, 1, 0);
		exit(EXIT_FAILURE);
	}
	ft_fork_1(pipex, envp);
	ft_fork_2(pipex, envp);
	free_close(pipex, 0, 1, 1);
	waitpid(pipex->cpid1, NULL, 0);
	waitpid(pipex->cpid2, NULL, 0);
	free_close(pipex, 1, 0 ,0);
}

/*
infile cmd1 | cmd2 outfile
cmd1 is writing it's stdout to the pipe
the stdin of grep is set to be on the pipe

infile --stdin--> cmd1 --stdout--> pipe
pipe   --stdin--> cmd2 --stdout--> outfile

2 processes are forked for cmd1 and cmd2

pipefd[0] -- read end of the pipe
pipefd[1] -- write end of the pipe

--> cmd1 process
- stdin is routed from infile
- stdout is routed to pipe
- infile fd is closed, outfile fd is closed, not used
- pipe fds are closed
- execute command with execve, rest of the code stops running

--> cmd2 process
- stdin is routed from pipe
- stdout is routed to outfile
- infile, outfile fd are closed
- pipe fds are closed
- execute command with execve, rest of the code stops running

--> main process
- close all file discreptors and pipe fds
- wait for child processes to execute
*/
