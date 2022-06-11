#include "../../INCL/minishell.h"

int	ft_cd(t_bash **bash, char **cmd, char **envp)
{
	char	*dir;
	int		res;
	char	new_pwd[256];

	if (cmd[1] == NULL)
		dir = find_it(envp, "HOME");
	else
	{
		if (cmd[2])
		{
			perror("cd: too many arguments\n");
			return (1);
		}
		dir = ft_strdup(cmd[1]);
	}
	res = chdir(dir);
	getcwd(new_pwd, sizeof(new_pwd));	//USARE FUNZIONI DI EXPORT PER AGGIORNARE VARIABILI PWD//
	// printf("%s\n", new_pwd);
	free(dir);
	if (res == -1)
		return (errno);					// aggiornare variabile globale
	return (0);
}
