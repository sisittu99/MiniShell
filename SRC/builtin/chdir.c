#include "../../INCL/minishell.h"

int	ft_cd(t_bash **bash, char **cmd, char **envp)
{
	char	*dir;
	int		*index;
	int		res;
	char	*new_pwd;

	if (cmd[1] == NULL)
	{
		index = find_it(envp, "HOME");
		dir = ft_substr(envp[index[0]], index[1] + 1,
						ft_strlen(envp[index[0]]));
	}
	else
	{
		if (cmd[2])
		{
			perror("cd: too many arguments\n");
			return (1);
		}
		dir = ft_strdup(cmd[1]);
	}
	if (((*bash)->pipe[0] == 0 && (*bash)->pipe[1] == 0)
		&& (*bash)->next == NULL)
		(*bash)->envp =ft_new_env(envp, 0);
	{
		new_pwd = (char *) malloc (sizeof(char) * 256);
		getcwd(new_pwd, 256);
		index = find_it(envp, "OLDPWD");
		ft_env_var_found(ft_strjoin("OLDPWD=", new_pwd), bash, index);
		free(index);
		res = chdir(dir);
		free(dir);
		if (res == -1)
			return (errno);					// aggiornare variabile globale
		free(new_pwd);
		new_pwd = (char *) malloc (sizeof(char) * 256);
		getcwd(new_pwd, 256);
		index = find_it(envp, "PWD");
		ft_env_var_found(ft_strjoin("PWD=", new_pwd), bash, index);
		free(new_pwd);
		free(index);
	}
	return (0);
}
