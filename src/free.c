/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-safa <wel-safa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 00:26:07 by wel-safa          #+#    #+#             */
/*   Updated: 2024/03/14 17:57:26 by wel-safa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*if (free_a): frees cmd1 and cmd2 and their options
if (close_f): closes infile and outfile
if (close_p): closes pipe ends*/
void	free_close(t_pipex *pipex, int free_a, int close_f, int close_p)
{
	if (free_a)
	{
		free(pipex->cmd1);
		free(pipex->cmd2);
		ft_free_splits(pipex->cmd_av_1);
		ft_free_splits(pipex->cmd_av_2);
		free(pipex->infilestr);
	}
	if (close_f)
	{
		if (pipex->infile && pipex->infile != -1)
			close(pipex->infile);
		if (pipex->outfile && pipex->outfile != -1)
			close(pipex->outfile);
	}
	if (close_p)
	{
		close(pipex->pipefd[0]);
		close(pipex->pipefd[1]);
	}
}

/*frees split memory allocs*/
void	ft_free_splits(char **splits)
{
	size_t	i;

	i = 0;
	while (splits[i])
		free(splits[i++]);
	free(splits[i]);
	free(splits);
}
