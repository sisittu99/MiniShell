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

	if (access(cmd, F_OK) == 0)
			return (cmd);
	while (*path)
	{
		tmp = ft_strjoin(*path, "/");
		address = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(address, F_OK) == 0)
			return (address);
		free (address);
		path++;
	}
	// perror("No access to command\n");
	// exit(127);
	return (NULL);
}
