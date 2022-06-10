#include "../../INCL/minishell.h"

int	ft_cd(char **cmd, char **envp)
{
	char	*dir;
	int		res;

	if (cmd[2] != NULL)
	{
		perror("cd: too many arguments\n");
		return (1);
	}
	if (cmd[1] == NULL)
		dir = find_it(envp, "HOME");
	else
		dir = ft_strdup(cmd[1]);
	res = chdir(dir);
	free(dir);
	if (res == -1)
		return (errno);					// aggiornare variabile globale
	return (0);

}
