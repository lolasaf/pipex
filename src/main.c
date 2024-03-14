/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-safa <wel-safa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:41:17 by wel-safa          #+#    #+#             */
/*   Updated: 2024/03/14 16:42:57 by wel-safa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_pipex(t_pipex *pipex)
{
	pipex->cmd1 = NULL;
	pipex->cmd2 = NULL;
	pipex->cmd_av_1 = NULL;
	pipex->cmd_av_2 = NULL;
	pipex->cpid1 = 0;
	pipex->cpid2 = 0;
	pipex->infile = 0;
	pipex->outfile = 0;
	pipex->pipefd[0] = 0;
	pipex->pipefd[1] = 0;
	pipex->status1 = 0;
	pipex->status2 = 0;
}

void	start_files(int argc, char **argv, t_pipex *pipex)
{
	if (argc != 5)
	{
		write(2, "Error: Program arguments are not equal to 4\n", 45);
		exit(EXIT_FAILURE);
	}
	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile < 0)
	{
		write(2, "Error: ", 8);
		write(2, "Outfile cannot be written to, created, or truncated\n", 53);
		exit(EXIT_FAILURE);
	}
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile < 0)
	{
		write(2, "Error: Infile not found or cannot be read\n", 43);
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	init_pipex(&pipex);
	start_files(argc, argv, &pipex);
	ft_pipex(argv, envp, &pipex);
	if (WIFEXITED(pipex.status2))
		return (WEXITSTATUS(pipex.status2));
	return (EXIT_FAILURE);
}
