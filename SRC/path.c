#include "../INCL/minishell.h"

char	**ft_path(char **envp)
{
	int		index[3];
	int		i;
	char	*to_find;

	i = 0;
	while (i < 3)
		index[i++] = 0;
	to_find = "PATH";
	while (envp[index[0]])
	{
		while (envp[index[0]][index[1]] == to_find[index[2]])
		{
			if (!to_find[index[2] + 1])
				return (ft_split(envp[index[0]], ':'));
			index[1]++;
			index[2]++;
		}
		index[0]++;
	}
	return (NULL);
}

char	*ft_access(char *cmd, char **path)
{
	char	*address;
	char	*tmp;
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
		return (cmd);
	}
	if (path == NULL)
		return (NULL);
	while (*path)
	{
		tmp = ft_strjoin(*path, "/");
		address = ft_strjoin(tmp, cmd);
		free(tmp);
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
	fd_printf(2, "bash: %s: No such file or directory\n", cmd);
	exit(127);
}
