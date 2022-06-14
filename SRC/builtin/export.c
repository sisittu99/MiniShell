#include "../../INCL/minishell.h"

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

void	ft_env_var_found(char *cmd, t_bash **bash, int index[3])
{
	char	*tmp;

	if (!ft_strchr(cmd, '='))
		return ;
	tmp = ft_strdup((*bash)->envp[index[0]]);
	free((*bash)->envp[index[0]]);						//FUNZIONA ?? SI!!!
	if (cmd[index[2]] == '+' && (*bash)->envp[index[0]][index[1] + 1] == '=')
		(*bash)->envp[index[0]] = ft_strjoin(tmp, ft_substr(cmd, index[2] + 2, ft_strlen(cmd) - index[2] - 1));
	else
		(*bash)->envp[index[0]] = ft_strdup(cmd);
	free(tmp);
	return ;
}

void	ft_handle_env(char *cmd, t_bash **bash)
{
	int		index[3];
	int		i;
	char	*to_find;
	char	**tmp;

	i = 0;
	while (i < 3)
		index[i++] = 0;
	to_find = cmd;
	if (ft_strchr(cmd, '=') != NULL)
	{
		i = 0;
		while (cmd[i++] != '=') ;
		to_find = ft_substr(cmd, 0, i);
	}
	while ((*bash)->envp[index[0]])
	{
		while ((*bash)->envp[index[0]][index[1]] == to_find[index[2]])
		{
			if (!to_find[index[2] + 1])
			{
				ft_env_var_found(cmd, bash, index);
				return ;
			}
			index[1]++;
			index[2]++;
		}
		index[0]++;
	}
	// caso in cui la variabile non viene trovata
	tmp =  ft_new_env((*bash)->envp, 1);
	tmp[index[0]] = ft_strdup(cmd);
	tmp[index[0] + 1] = NULL;
	ft_free((*bash)->envp);
	(*bash)->envp = ft_new_env(tmp, 0);
	return ;
}

char	**ft_sort_env(char **envp)
{
	char	**sort;
	char	*tmp;
	int		i;
	int		j;
	int		x;

	i = 0;
	j = 0;
	x = 1;
	while (envp[i++]);
	sort = (char **) malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (envp[++i])
		sort[i] = ft_strdup(envp[i]);
	sort[i] = NULL;
	while (sort[j])
	{
		while (x < i - j - 1)
		{
			if (ft_strncmp(sort[x + 1], sort[x]) < 0)
			{
				tmp = ft_strdup(sort[x + 1]);
				sort[x + 1] = ft_strdup(sort[x]);
				sort[x] = ft_strdup(tmp);
			}
			x++;
		}
		x = 0;
		j++;
	}
	sort[i] = NULL;
	return (sort);
}

int	ft_lonely_export(char **envp)
{
	char	**tmp;
	int		i;
	int		j;

	i = 1;
	j = 0;
	tmp = ft_new_env(envp, 0);
	tmp = ft_sort_env(envp);
	while (tmp[i])
	{
		ft_putstr_fd("declare -x ", 1);
		while (tmp[i][j])
		{
			if (tmp[i][j] == '=')
			{
				ft_putchar_fd(tmp[i][j++], 1);
				write(1, "\"", 1);
				while (tmp[i][j])
					ft_putchar_fd(tmp[i][j++], 1);
				write(1, "\"", 1);
			}
			else
				ft_putchar_fd(tmp[i][j++], 1);
		}
		ft_putchar_fd('\n', 1);
		j = 0;
		i++;
	}
	return (0);
}

int	ft_export(t_bash **bash, char **cmd, char **envp)
{
	int		i;
	int		j;
	int		status;

	i = 1;
	j = 0;
	if (cmd[1] == NULL)
		return (ft_lonely_export(envp));
	if (((*bash)->pipe[0] == 0 && (*bash)->pipe[1] == 0)
		&& (*bash)->next == NULL)
		(*bash)->envp = ft_new_env(envp, 0);
	status = 0;
	while (cmd[i])
	{
		while (cmd[i][j])
		{
			if (!ft_isalpha(cmd[i][j]) && !ft_isdigit(cmd[i][j]) && cmd[i][j] != '_')
			{
				fd_printf(2, "export: `%s\': not a valid identifier\n", cmd[i]);
				status = 1;
				break ;
			}
			else if ((cmd[i][j + 1] == '\0' || cmd[i][j + 1] == '=')
						&& ((*bash)->pipe[0] == 0 && (*bash)->pipe[1] == 0)
						&& (*bash)->next == NULL)
			{
				ft_handle_env(cmd[i], bash);
				break ;
			}
			j++;
		}
		j = 0;
		i++;
	}
	return (status);
}
