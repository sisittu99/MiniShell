#include "../INCL/minishell.h"

void	ft_free(char **dc)
{
	int	i;

	i = -1;
	while (dc[++i])
		free(dc[i]);
	free(dc);
}

int	main(int argc, char *argv[], char *envp[])
{
	char *line;
	char **cmd;
	int	i;

	(void)argc;
	(void)argv;
	while (1)
	{
		line = readline("bash-biutiful>$ ");
		if (!line)
			return(write(2, "error: could not allocate!\n", 28));
		if (*line)
		{
			add_history(line);
			cmd = ms_split(line, ' ', envp);
			i = 0;
			while (cmd[i] != NULL)
				printf("%s$\n", cmd[i++]);
			// ft_free(cmd);
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
