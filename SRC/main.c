#include "../INCL/minishell.h"

void	ft_free(char **dc)
{
	int	i;

	i = -1;
	while (dc[++i])
		free(dc[i]);
	free(dc);
}

void	ft_sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
	}
	rl_on_new_line();
	rl_redisplay();
	return ;
}

void	ft_control_d(char *line)
{
	free(line);
	printf("Exit\n");
	exit (0);
}

int	main(int argc, char **argv, char **envp)
{
	char				*line;
	t_bash				*bash;
	struct sigaction	sa;
	char				*tmp;

	(void)argv;
	if (argc != 1)
	{
		write(2, "error: no argument allowed\n", 28);
		return (1);
	}
	bash = NULL;
	sa.sa_handler = ft_sig_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	while (1)
	{
		line = readline("bash-biutiful>$ ");
		if (!line)
			ft_control_d(line);
		if (*line)
		{
			ft_parse(&bash, line, envp);
			ft_execute(&bash, envp, &line);
			if (tmp == NULL || ft_strcmp(tmp, line) == 0)
			{
				add_history(line);
				if (tmp)
					free(tmp);
				tmp = ft_strdup(line);
			}
			//CHECK ENPV IN STRUCT SE ESISTE -> IN CASO MODIFICA ENVP REALE
			if (bash->envp)
				envp = ft_new_envp(bash->envp, envp);
			ft_delete_lst(&bash);
		}
		free(line);
	}
	free(tmp);
	return (0);
}
