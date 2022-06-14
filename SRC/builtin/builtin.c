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

int	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '=') != 0)
			printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

void	ft_exit(char **cmd)
{
	int	i;

	if (!cmd[1])
	{
		printf("\nExit\n");
		exit(exit_status);
	}
	if (cmd[2])
	{
		fd_printf(2, "exit: too many arguments\n");
		return ;
	}
	i = -1;
	while (cmd[1][++i])
	{
		if (!ft_isdigit(cmd[1][i]))
		{
			fd_printf(2, "\nExit\nexit: %s: numeric argument required\n", cmd[1]);
			exit(255);
		}
	}
	printf("\nExit\n");
	exit(ft_atoi(cmd[1]));
}
