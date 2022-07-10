/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:47:45 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/22 16:47:45 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

char	**ft_path(char **envp)
{
	int		*index;

	index = find_it(envp, "PATH");
	if (index != NULL)
		return (ft_split(ft_substr(envp[index[0]], 5
					, ft_strlen(envp[index[0]]) - 5), ':'));
	return (NULL);
}

int	ft_formatted_addr(char *cmd)
{
	DIR		*t;

	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) != 0)
		{
			fd_printf(2, "bash: %s: Permission denied\n", cmd);
			exit(126);
		}
		else
		{
			t = opendir(cmd);
			if (t != NULL)
			{
				closedir(t);
				fd_printf(2, "bash: %s: is a directory\n", cmd);
				exit(126);
			}
		}
		return (1);
	}
	return (0);
}

char	*ft_access_join(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*dst;

	tmp = ft_strjoin(s1, s2);
	dst = ft_strjoin(tmp, s3);
	free(tmp);
	return (dst);
}

char	*ft_access(char *cmd, char **path)
{
	char	*address;

	if (ft_formatted_addr(cmd))
		return (cmd);
	if (ms_strchr(cmd, 0, '/') != -1)
	{
		fd_printf(2, "bash: %s: No such file or directory\n", cmd);
		exit(127);
	}
	while (path && *path)
	{
		address = ft_access_join(*path, ft_strdup("/"), cmd);
		if (access(address, F_OK) == 0)
		{
			if (access(address, X_OK) != 0)
			{
				fd_printf(2, "bash: %s: Permission denied\n", cmd);
				exit(126);
			}
			return (address);
		}
		free (address);
		path++;
	}
	return (NULL);
}
