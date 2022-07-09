/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcerchi <mcerchi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 12:09:55 by fdrudi            #+#    #+#             */
/*   Updated: 2022/07/09 17:44:53 by mcerchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		// rl_replace_line("", 0);
	}
	rl_on_new_line();
	rl_redisplay();
	return ;
}

void	ft_control_d(char *line, char **envp, t_bash **bash)
{
	ft_free(envp);
	ft_delete_lst(bash);
	printf("\nExit\n");
	free(line);
	exit (g_exit_status);
}

void	ft_sig_default(int sig)
{
	if (sig == SIGINT)
	{
		// rl_replace_line("", 0);
	}
	rl_on_new_line();
	rl_redisplay();
	if (sig == SIGINT)
	{
		printf("\n");
		exit(0);
	}
}
