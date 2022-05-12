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
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	printf("\nbash-biutiful>$ ");
	(void) sig;
	// free(line);
	// line = NULL;
	// rl_replace_line("", 0);
	return ;
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*line;
	t_bash	*bash;
	struct sigaction	sa;

	sa.sa_handler = ft_sig_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	(void)argc;
	(void)argv;
	(void)envp;
	bash = NULL;
	while (1)
	{
		line = readline("bash-biutiful>$ ");
		if (!line)
			return(write(2, "error: could not allocate!\n", 28));
		if (*line)
		{
			ft_parse(&bash, line, envp);
			ft_execute(&bash, envp, &line);
			add_history(line);
			ft_delete_lst(&bash);
		}
	}
	return (0);
}

// ** SIGINT HANDLER ** //

// static void	int_handler(int status) {
// 	printf("\n"); // Move to a new line
// 	rl_on_new_line(); // Regenerate the prompt on a newline
// 	rl_replace_line("", 0); // Clear the previous text
// 	rl_redisplay();
// }

// ** EXEC EXAMPLE ** //

// int pid = fork();
// 	if (pid == 0)
// 	{
// 		char *path = ft_access("ls", ft_path(envp));
// 		char *tmp[] = {"ls", "la", NULL};
// 		int i = execve(path, tmp, envp);
// 		printf ("Exec Failed with: %d\n", i);
// 		return (0);
// 	}

// 	wait(NULL);
// 	printf("Exec Finished !\n");
