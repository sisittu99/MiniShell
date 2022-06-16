#include "../../INCL/minishell.h"

int	ft_pwd(char **envp)
{
	int		*index;

	index = find_it(envp, "PWD");
	printf("%s\n", ft_substr(envp[index[0]],
				index[1] + 2, ft_strlen(envp[index[0]]) - index[1]));
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
	if (cmd[1][0] == '-')
		i++;
	while (cmd[1][++i])
	{
		if (!ft_isdigit(cmd[1][i]))
		{
			fd_printf(2, "\nExit\nbash: exit: %s: numeric argument required\n", cmd[1]);
			exit(255);
		}
	}
	printf("\nExit\n");
	exit(ft_atoi(cmd[1]));
}
