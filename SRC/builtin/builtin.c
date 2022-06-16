#include "../../INCL/minishell.h"

int	ft_pwd(char **cmd, char **envp)
{
	int		*index;

	if (ft_invalid_option(cmd) == 1)
		return (1);
	index = find_it(envp, "PWD");
	if (index != NULL)
		printf("%s\n", ft_substr(envp[index[0]],
				index[1] + 2, ft_strlen(envp[index[0]]) - index[1]));
	return (0);
}

int	ft_env(char **cmd, char **envp)
{
	int	i;

	i = 0;
	if (ft_invalid_option(cmd) == 1)
		return (1);
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
