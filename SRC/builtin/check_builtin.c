#include "../../INCL/minishell.h"

void	ft_exec_builtin(t_bash **bash, char **envp)
{
	(void)envp;
	if ((*bash)->built == 0)
		ft_echo((*bash)->cmd);
	else if ((*bash)->built == 1)
		ft_cd(bash, (*bash)->cmd, envp);
	else if ((*bash)->built == 2)
		ft_pwd(envp);
	else if ((*bash)->built == 3)
		ft_export(bash, (*bash)->cmd, envp);
	else if ((*bash)->built == 4)
		ft_unset(bash, (*bash)->cmd, envp);
	else if ((*bash)->built == 5)
		ft_env(envp);
	else if ((*bash)->built == 6)
		printf("exit\n"); // ft_exit();
	if ((*bash)->next != NULL)
		exit(0);
	return ;
}

/* Crea la matrice con la quale controllare che il comando sia una built-in,
	scritta correttamente */
char	**ft_builtin_assgn(void)
{
	char	**built;

	built = (char **) malloc (sizeof(char *) * 8);
	if (!built)
		exit(errno);
	built[0] = ft_strdup("echo");
	built[1] = ft_strdup("cd");
	built[2] = ft_strdup("pwd");
	built[3] = ft_strdup("export");
	built[4] = ft_strdup("unset");
	built[5] = ft_strdup("env");
	built[6] = ft_strdup("exit");
	built[7] = 0;
	return (built);
}

/* Controlla che il comando sia una built-in.
	In caso positivo, ritorna la pos nella matrice built
	per poter proseguire ed eseguire la funzione corretta. */
int	ft_check_builtin(char *cmd)
{
	int		i;
	char	**built;

	built = ft_builtin_assgn();
	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(cmd, built[i]) == 1)
			return (i);
		i++;
	}
	return (-1);
}
