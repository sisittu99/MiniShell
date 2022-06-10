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
	exit (0);
}

void	ft_sig_default(int sig)
{
	(void)sig;
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
