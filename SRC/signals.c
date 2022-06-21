#include "../INCL/minishell.h"

void	ft_sig_define(struct sigaction *sa, int def)
{
	if (def == 1)
	{
		sa->sa_handler = SIG_IGN;
		sa->sa_flags = SA_RESTART;
		sigaction(SIGINT, sa, NULL);
		sigaction(SIGQUIT, sa, NULL);
		return ;
	}
	sa->sa_handler = ft_sig_handler;
	sa->sa_flags = SA_RESTART;
	sigaction(SIGINT, sa, NULL);
	sigaction(SIGQUIT, sa, NULL);
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
	printf("\nExit\n");
	free(line);
	exit (g_exit_status);
}

void	ft_sig_default(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
	}
	rl_on_new_line();
	rl_redisplay();
	if (sig == SIGINT)
	{
		printf("\n");
		exit(0);
	}
}

/*
	Crea o modifica il file ~/.inputrc, nel quale si spegne la stampa dei caratteri di controllo.
*/
void	ft_rm_ctrl(char **envp)
{
	int		i;
	int		fd;
	char	*tmp;

	i = 0;
	fd = open(ft_strjoin(ft_replace_tilde(ft_strdup("~"), envp, 0, &i),
		 "/.inputrc"), O_RDWR | O_CREAT | O_APPEND, 0777);
	tmp = ft_get_next_line(fd);
	while (tmp)
	{
		if (ft_strcmp(tmp, ft_strdup("set echo-control-characters Off\n")) == 1)
		{
			close(fd);
			free(tmp);
			return ;
		}
		tmp = ft_get_next_line(fd);
	}
	fd_printf(fd, "set echo-control-characters Off\n");
	close(fd);
	free(tmp);
}
