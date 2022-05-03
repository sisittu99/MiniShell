#include "../INCL/minishell.h"
/*
	esegue un comando in una sola riga.
	Questa funzione dovrá essere implementata. La base cosí di fatto puó funzionare solo 
*/
void	ft_execute(t_bash **bash, char **envp)
{
	t_bash	*tmp;

	tmp = *bash;
	while (tmp)
	{
		(tmp)->proc = fork();
		if ((tmp)->proc < 0)
			exit(errno);
		else if ((tmp)->proc == 0)
		{
			if (execve(ft_access((tmp)->cmd[0], ft_path(envp)), (tmp)->cmd, envp) == -1)
				write(2, "does not work man\n", 19);
			exit(errno);
		}
		waitpid((tmp)->proc, NULL, 0);
		tmp = (tmp)->next;
	}
	return ;
}
