#include "../INCL/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	char *line;
	char *prompt;
	while (1)
	{
		printf("bash-biutiful>$ ");
		line = readline(prompt);
		if (line && *line)
		{
			add_history(line);
			printf("%s\n", line);
		}
		free(line);
	}
	return (0);
}

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
