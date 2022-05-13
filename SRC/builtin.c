#include "../INCL/minishell.h"

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
			if (!to_find[index[2] + 1])
				return (printf("%s\n", ft_substr(envp[index[0]],
								index[1] + 1, ft_strlen(envp[index[0]]) - index[1])));
			index[1]++;
			index[2]++;
		}
		index[0]++;
	}
	return (NULL);
}

void	ft_echo(char **cmd)
{
	int		i;
	int		flag;

	i = 1;
	flag = 0;
	if (ft_strcmp(cmd[0], "echo") == 0)
	{
		if (ft_strcmp(cmd[1], "-n") == 0)
		{
			flag = 1;
			i++;
		}
		while (cmd[i + 1])
			printf("%s ", cmd[i++]);
		printf("%s", cmd[i]);
		if (flag == 0)
			printf("\n");
	}
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
}

void	ft_exit(void)
{
	printf("exit\n");
	exit(0);
}


