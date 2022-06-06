#include "../INCL/minishell.h"

void	ft_free(char **dc)
{
	int	i;

	i = -1;
	if (dc[0])
	{
		while (dc[++i])
			free(dc[i]);
	}
	if (dc)
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

char	**ft_new_env(char **mat, int def)
{
	char	**new;
	int		i;

	if (!mat)
		return (NULL);
	i = 0;
	while (mat[i++]) ;
	new = (char **) malloc (sizeof(char *) * i + def);
	if (!new)
		return (NULL);
	i = 0;
	while (mat[i])
	{
		new[i] = ft_strdup(mat[i]);
		i++;
	}
	new[i] = NULL;
	return (new);
}

int	main(int argc, char **argv, char **envp)
{
	t_bash				*bash;
	char				*line;
	char				*tmp;
	char				**env;
	struct sigaction	sa;

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
	env = ft_new_env(envp, 0);
	while (1)
	{
		line = readline("bash-biutiful>$ ");
		if (!line)
			ft_control_d(line);
		if (*line)
		{
			ft_parse(&bash, line, env);
			if (bash->next != NULL)
			{									//////////////
				signal(SIGINT, SIG_DFL);  //////DA SISTEMARE !!!////////
				signal(SIGQUIT, SIG_DFL);		/////////////
			}
			ft_execute(&bash, env, &line);
			if (tmp == NULL || ft_strcmp(tmp, line) == 0)
			{
				add_history(line);
				if (tmp)
					free(tmp);
				tmp = ft_strdup(line);
			}
			//CHECK ENPV IN STRUCT SE ESISTE -> IN CASO MODIFICA ENVP REALE
			if (bash->envp)
			{
				ft_free(env);
				env = ft_new_env(bash->envp, 0);
			}
			ft_delete_lst(&bash);
		}
		free(line);
	}
	free(tmp);
	return (0);
}
