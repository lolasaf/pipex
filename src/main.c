/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-safa <wel-safa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:41:17 by wel-safa          #+#    #+#             */
/*   Updated: 2024/03/03 20:37:44 by wel-safa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	char	**execpath;

	argc++;
	printf("%s\n", argv[0]);
	execpath = ft_execpath("ls", envp);
	printf("Found path: %s\n", execpath);
	free(execpath);
	return (0);
}
