/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execpath.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-safa <wel-safa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 20:14:17 by wel-safa          #+#    #+#             */
/*   Updated: 2024/03/11 20:50:29 by wel-safa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	perror("PATH not found in envp\n");
	return (NULL);
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
			free(tmp_path);
			perror("tmp_path malloc error in ft_exec_path_find\n");
			return (NULL);
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

/*Looks for executable file "exec" in envp (environment variables)
and returns a malloced memory containing its path
*/
char	*ft_execpath(char *exec, char **envp)
{
	char	*pathenv;
	char	**pathsplit;
	char	*exec_path;

	pathenv = ft_get_pathenv(envp);
	if (pathenv == NULL)
	{
		perror("pathenv is NULL in ft_execpath\n");
		return (NULL);
	}
	pathsplit = ft_split(pathenv + 5, ':');
	printf("exec= %s\n", exec);
	exec_path = ft_exec_path_find(pathsplit, exec);
	ft_free_splits(pathsplit);
	if (exec_path)
		return (exec_path);
	free(exec_path);
	perror("An executable path was not found\n");
	return (NULL);
}
