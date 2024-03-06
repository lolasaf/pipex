/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-safa <wel-safa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:41:17 by wel-safa          #+#    #+#             */
/*   Updated: 2024/03/07 00:30:07 by wel-safa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
	{
		perror("Program arguments are not equal to 4\n");
		exit(EXIT_FAILURE);
	}
	pipex.infile = open(argv[1], O_RDONLY);
	if (pipex.infile < 0)
	{
		perror("Infile not found or cannot be read\n");
		exit(EXIT_FAILURE);
	}
	pipex.outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex.outfile < 0)
	{
		perror("Outfile cannot be written to, created, or truncated\n");
		close(pipex.infile);
		exit(EXIT_FAILURE);
	}
	ft_pipex(argv, envp, &pipex);
	return (0);
}
