
#include "../../INCL/minishell.h"



char	**ft_delete_env_var( char **envp, int index[3])
{
	while (envp[index[0] + 1])
	{
		envp[index[0]] = envp[index[0] + 1];
		index[0] += 1;
	}
	envp[index[0]] = NULL;
	return (envp);
}

char	**ft_unset(char *cmd, char **envp)
{
	int		index[3];
	int		i;
	char	*to_find;

	if (cmd[5] == '\0')
		return (envp);
	i = 0;
	while (i < 3)
		index[i++] = 0;
	while (envp[index[0]])
	{
		while (envp[index[0]][index[1]] == to_find[index[2]])
		{
			if (!to_find[index[2] + 1])
				return (ft_delete_env_var(envp, index));
			index[1]++;
			index[2]++;
		}
		index[0]++;
	}
	return (envp);
}
