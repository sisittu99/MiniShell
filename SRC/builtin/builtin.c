#include "../../INCL/minishell.h"

int	ft_pwd(char **envp)
{
	int		index[3];
	int		i;
	char	*to_find;

	i = 0;
	while (i < 3)
		index[i++] = 0;
	to_find = "PWD";
	while (envp[index[0]])
	{
		while (envp[index[0]][index[1]] == to_find[index[2]])
		{
			if (!to_find[index[2] + 1])										/////////////////////////////////////////////////////
				return (printf("%s\n", ft_substr(envp[index[0]],         //SE USASSIMO " printf("%s\n", find_it(envp, to_find)) " ?? //
							index[1] + 2, ft_strlen(envp[index[0]]) - index[1]))); /////////////////////////////////////////
			index[1]++;
			index[2]++;
		}
		index[0]++;
	}
	return (0);
}

void	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '=') != 0)
			printf("%s\n", envp[i]);
		i++;
	}
	return ;
}

void	ft_exit(void)
{
	printf("Exit\n");
	exit(0);
}
