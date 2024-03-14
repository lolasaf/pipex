/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-safa <wel-safa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 23:59:57 by wel-safa          #+#    #+#             */
/*   Updated: 2024/03/14 18:24:33 by wel-safa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_fork_1_1(t_pipex *pipex, char **envp)
{
	dup2(pipex->infile, STDIN_FILENO);
	dup2(pipex->pipefd[1], STDOUT_FILENO);
	free_close(pipex, 0, 1, 1);
	if (pipex->cmd1 == NULL)
	{
		free_close(pipex, 1, 0, 0);
		exit(EXIT_FAILURE);
	}
	execve(pipex->cmd1, pipex->cmd_av_1, envp);
	write(2, "Error: execve\n", 15);
	free_close(pipex, 1, 0, 0);
	exit(EXIT_FAILURE);
}

/*Child process 1*/
void	ft_fork_1(t_pipex *pipex, char **envp)
{
	pipex->cpid1 = fork();
	if (pipex->cpid1 < 0)
	{
		write(2, "Error: Fork error\n", 19);
		free_close(pipex, 1, 1, 1);
		exit(EXIT_FAILURE);
	}
	if (pipex->cpid1 == 0)
	{
		pipex->infile = open(pipex->infilestr, O_RDONLY);
		if (pipex->infile < 0)
		{
			write(2, "Error: Infile not found or cannot be read\n", 43);
			free_close(pipex, 1, 1, 1);
			exit(EXIT_FAILURE);
		}
		ft_fork_1_1(pipex, envp);
	}
}

/*Child process 2*/
void	ft_fork_2(t_pipex *pipex, char **envp)
{
	pipex->cpid2 = fork();
	if (pipex->cpid2 < 0)
	{
		write(2, "Error: Fork error\n", 19);
		free_close(pipex, 1, 1, 1);
		exit(EXIT_FAILURE);
	}
	if (pipex->cpid2 == 0)
	{
		dup2(pipex->pipefd[0], STDIN_FILENO);
		dup2(pipex->outfile, STDOUT_FILENO);
		free_close(pipex, 0, 1, 1);
		execve(pipex->cmd2, pipex->cmd_av_2, envp);
		write(2, "Error: execve\n", 15);
		free_close(pipex, 1, 0, 0);
		exit(EXIT_FAILURE);
	}
}

void	ft_pipex_args(char **argv, t_pipex *pipex)
{
	pipex->cmd_av_1 = ft_split(argv[2], ' ');
	pipex->cmd_av_2 = ft_split(argv[3], ' ');
	if (pipex->cmd_av_1 == NULL || pipex->cmd_av_2 == NULL)
	{
		write(2, "Error: Split error\n", 20);
		free_close(pipex, 1, 1, 0);
		exit(EXIT_FAILURE);
	}
	if (pipex->cmd_av_2[0] == NULL)
	{
		write(2, "Error: Command input is not valid\n", 35);
		free_close(pipex, 1, 1, 0);
		exit(EXIT_FAILURE);
	}
	if (!ft_strlen(pipex->cmd_av_2[0]))
	{
		write(2, "Error: Command input is not valid\n", 35);
		free_close(pipex, 1, 1, 0);
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
	ft_pipex_args(argv, pipex);
	pipex->cmd1 = ft_execpath(pipex, pipex->cmd_av_1[0], envp);
	pipex->cmd2 = ft_execpath(pipex, pipex->cmd_av_2[0], envp);
	if (pipex->cmd2 == NULL)
	{
		free_close(pipex, 1, 1, 0);
		exit(127);
	}
	if (pipe(pipex->pipefd) < 0)
	{
		write(2, "Error: Pipe error\n", 19);
		free_close(pipex, 1, 1, 0);
		exit(EXIT_FAILURE);
	}
	ft_fork_1(pipex, envp);
	ft_fork_2(pipex, envp);
	free_close(pipex, 1, 1, 1);
	waitpid(pipex->cpid1, &pipex->status1, 0);
	waitpid(pipex->cpid2, &pipex->status2, 0);
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
