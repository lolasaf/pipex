/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-safa <wel-safa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:41:17 by wel-safa          #+#    #+#             */
/*   Updated: 2024/03/05 01:23:52 by wel-safa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void free_close(char *cmd1, char *cmd2, pid_t infile, pid_t outfile)
{
	free(cmd1);
	free(cmd2);
	close(infile);
	close(outfile);
}

void	pipex(char **argv, char **envp, int infile, int outfile)
{
	char	*cmd1;
	char	*cmd2;
	int		pipefd[2];
	//pid_t	cpid1;
	//pid_t	cpid2;

	cmd1 = ft_execpath(argv[2], envp);
	cmd2 = ft_execpath(argv[3], envp);
	if (cmd1 == NULL || cmd2 == NULL)
	{
		perror("one or two executable files were not found\n");
		free_close(cmd1, cmd2, infile, outfile);
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd) < 0)
	{
		perror("Pipe error\n");
		free_close(cmd1, cmd2, infile, outfile);
		exit(EXIT_FAILURE);
	}
	/*cpid1 = fork();
	if (cpid1 < 0)
	{
		perror("Fork error\n");
		free_close(cmd1, cmd2, infile, outfile);
		exit(EXIT_FAILURE);
	}
	*/
	free_close(cmd1, cmd2, infile, outfile);
}

int	main(int argc, char **argv, char **envp)
{
	int		infile;
	int		outfile;

	if (argc != 5)
	{
		perror("Program arguments are not equal to 4\n");
		exit(EXIT_FAILURE);
	}
	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
	{
		perror("Infile not found or cannot be read\n");
		exit(EXIT_FAILURE);
	}
	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
	{
		perror("Outfile cannot be written to, created, or truncated\n");
		close(infile);
		exit(EXIT_FAILURE);
	}
	pipex(argv, envp, infile, outfile);
	return (0);
}
