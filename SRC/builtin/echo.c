#include "../../INCL/minishell.h"

int	ft_flag_echo(char **cmd)
{
	int	i;
	int	j;
	int	flag;

	i = 1;
	flag = 0;
	while (cmd[i] && cmd[i][0] == '-')
	{
		j = 1;
		while (cmd[i][j])
		{
			if (cmd[i][j] != 'n')
				return (flag);
			j++;
		}
		flag++;
		i++;
	}
	return (flag);
}


int	ft_echo(char **cmd)
{
	int		i;
	int		flag;

	i = 1;
	flag = 0;
	if (cmd[1] != NULL)
	{
		flag = ft_flag_echo(cmd);
		if (cmd[1 + flag])
		{
			// if (ft_strcmp(cmd[1 + flag], "$?") == 1)
			// 	printf("%d", exit_status);
			// else
			// {
				while (cmd[i + flag + 1])
					printf("%s ", cmd[(i++) + flag]);
				printf("%s", cmd[i + flag]);
			// }
		}
	}
	if (flag == 0)
		printf("\n");
	return (0);
}
