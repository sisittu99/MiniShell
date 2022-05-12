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
		if (ft_strchr(envp[i], '=') == 0)
			printf("%s\n", envp[i]);
		i++;
	}
}

char **ft_handle_env(char *cmd, char **envp)
{
	int		index[3];
	int		i;
	char	*to_find;

	i = 0;
	while (i < 3)
		index[i++] = 0;
	to_find = cmd;
	if (ft_strchr(cmd, '=') != NULL)
	{
		i = 0;
		while (cmd[i++] != '=');
		to_find = ft_substr(cmd, 0, i);
	}
	while (envp[index[0]])
	{
		while (envp[index[0]][index[1]] == to_find[index[2]])
		{
			if (!to_find[index[2] + 1])
				;													/* controllare se ha un valore assegnato. se si, modificarlo, altrimenti assegnarlo (sempre che ci sia qualcosa da assegnare)*/
			index[1]++;
			index[2]++;
		}
		index[0]++;
	}
}

char	**ft_export(char **cmd, char **envp)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	if (cmd[1][0] == '\0')
	{
		while (envp[i])
			printf("declare -x %s\n", envp[i++]);
		return (NULL);
	}
	while (cmd[i])
	{
		while (cmd[i][j])
		{
			if (!ft_isalpha(cmd[i][j] && cmd[i][j] != '_'))
				printf("export: `%s\': not a valid identifier\n", cmd[i]);
			else
				envp = ft_handle_env(cmd[i], envp);
		}
		j = 0;
		i++;
	}
}


void	ft_exit(void)
{
	printf("exit\n");
	exit(0);
}


