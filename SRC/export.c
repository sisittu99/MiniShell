#include "../INCL/minishell.h"

/*
	Casistica:
							var con = :			|	var senza =:	|		var non presente:
	---------------------------------------------------------------------------------------------
	cmd senza segno:		return ;			|	return ;		|	env[last] = cmd; [OK]
	---------------------------------------------------------------------------------------------
	cmd con = :			env[i] = cmd;			|	env[i] = cmd;	|	env[last] = cmd; [OK]
	---------------------------------------------------------------------------------------------
	cmd con += : strjoin(env[i], substr(cmd...))|	env[i] = cmd;	|	env[last] = cmd; [OK]
*/

char	**ft_delete_env_var( char **envp, int index[3])
{
	while(envp[index[0] + 1])
	{
		envp[index[0]] = envp[index[0] + 1];
		index[0] += 1;
	}
	envp[index[0]] = NULL;
	return (envp);
}

char	**ft_unset(char *cmd, char **envp)
{
	if (cmd[5] == '\0')
		return (envp);
	int		index[3];
	int		i;
	char	*to_find;

	i = 0;
	while (i < 3)
		index[i++] = 0;
	while (envp[index[0]])
	{
		while (envp[index[0]][index[1]] == to_find[index[2]])
		{
			if (!to_find[index[2] + 1])
				return(ft_delete_env_var(envp, index));
			index[1]++;
			index[2]++;
		}
		index[0]++;
	}
	return (envp);
}

char	**ft_env_var_found(char *cmd, char **envp, int index[3])
{
	if (!ft_strchr(cmd, '='))
		return(envp);
	if (cmd[index[2]] == '+' && envp[index[0]][index[1] + 1] == '=')
		envp[index[0]] = ft_strjoin(envp[index[0]], ft_substr(cmd, index[2] + 2, ft_strlen(cmd) - index[2] - 1));
	else
		envp[index[0]] = cmd;
	return (envp);
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
				return(ft_env_var_found(cmd, envp, index));
			index[1]++;
			index[2]++;
		}
		index[0]++;
	}
	// caso in cui la variabile non viene trovata
	envp[i] = ft_strdup(cmd);
	envp[i + 1] = NULL;
	return (envp);
}

char **ft_sort_env(char **envp)
{
	char	**sort;
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (envp[i++]);
	sort = (char **) malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (envp[++i])
		sort[i] = ft_strdup(envp[i]);
	i = 0;
	while (sort[i + 1])
	{
		while (sort[i][j] && sort[i][j])
		{
			if (sort[i][j] > sort[i + 1][j])
			{
				tmp = ft_strdup(sort[i]);
				sort[i] = sort[i + 1];
				sort[i + 1] = tmp;
				break ;
			}
			else if (sort[i][j] == sort[i + 1][j])
				j++;
			else
				break ;
		}
		j = 0;
		i++;
	}
	return (sort);
}

char	**ft_export(char **cmd, char **envp)
{
	char	**tmp;
	int		i;
	int		j;

	i = 1;
	j = 0;
	if (cmd[1][0] == '\0')
	{
		tmp = ft_sort_env(envp);
		while (tmp[i])
			printf("declare -x %s\n", tmp[i++]);
		return (envp);
	}
	while (cmd[i])
	{
		while (cmd[i][j])
		{
			if (!ft_isalpha(cmd[i][j] && !ft_isdigit(cmd[i][j]) && cmd[i][j] != '_'))
			{
				printf("export: `%s\': not a valid identifier\n", cmd[i]);
				break ;
			}
			else if (cmd[i][j + 1] == '\0')
				envp = ft_handle_env(cmd[i], envp);
			j++;
		}
		j = 0;
		i++;
	}
}
