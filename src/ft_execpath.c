/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execpath.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-safa <wel-safa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 20:14:17 by wel-safa          #+#    #+#             */
/*   Updated: 2024/03/03 20:38:37 by wel-safa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*frees path split memory allocs*/
void	ft_free_pathsplit(char **splits)
{
	size_t	i;

	i = 0;
	while (splits[i])
		free(splits[i++]);
	free(splits[i]);
	free(splits);
}

/*Searches envp for string corresponding to PATH variable
and returns it, exits if not found*/
char	*ft_get_pathenv(char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (envp[i]);
	}
	perror("PATH not found in envp");
	exit(1);
}

/*Iterates over pathsplit, appends "/exec",
and checks if executable path is found and is accessible and returns it
returns NULL on error*/
char	*ft_exec_path_find(char **pathsplit, char *exec)
{
	int		i;
	char	*tmp_path;
	size_t	tmp_path_size;

	i = -1;
	while (pathsplit[++i])
	{
		tmp_path_size = ft_strlen(pathsplit[i]) + ft_strlen(exec) + 2;
		tmp_path = (char *)ft_calloc(sizeof(char), tmp_path_size);
		if (tmp_path == NULL)
		{
			perror("tmp_path malloc error in ft_exec_path_find");
			free(tmp_path);
			ft_free_pathsplit(pathsplit);
			exit(1);
		}
		ft_strlcat(tmp_path, pathsplit[i], tmp_path_size);
		ft_strlcat(tmp_path, "/", tmp_path_size);
		ft_strlcat(tmp_path, exec, tmp_path_size);
		if (access(tmp_path, X_OK) == 0)
			return (tmp_path);
		free(tmp_path);
	}
	return (NULL);
}

/*Looks for executable file "exec" and returns its path
envp = environment variables*/
char	*ft_execpath(char *exec, char **envp)
{
	char	*pathenv;
	char	**pathsplit;
	char	*exec_path;

	pathenv = ft_get_pathenv(envp);
	if (pathenv == NULL)
	{
		perror("pathenv is NULL in ft_execpath");
		exit(1);
	}
	pathsplit = ft_split(pathenv + 5, ':');
	exec_path = ft_exec_path_find(pathsplit, exec);
	if (exec_path)
		return (exec_path);
	free(exec_path);
	ft_free_pathsplit(pathsplit);
	perror("An executable path was not found");
	exit(1);
}
