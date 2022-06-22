/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:32:46 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/22 12:40:33 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Funzione Helper che chiude tutti gli fd creati dalle pipe <- */
void	ft_close_pipe(t_bash **bash)
{
	t_bash	*tmp;

	tmp = *bash;
	close(tmp->pipe[0]);
	close(tmp->pipe[1]);
	while ((tmp->next->pipe[0] != 0 && tmp->next->pipe[1] != 0)
		&& tmp->next != NULL)
	{
		tmp = tmp->next;
		close(tmp->pipe[0]);
		close(tmp->pipe[1]);
	}
}

/* -> Funzione helper che controlla ed esegue tutte le pipe intermediarie <- */
t_bash	*ft_pipe_help_b(t_bash **bash, t_bash **start, char **envp, char *line)
{
	t_bash	*tmp;

	tmp = *bash;
	while ((tmp->next->pipe[0] != 0 && tmp->next->pipe[1] != 0)
		&& tmp->next != NULL)
	{
		(*bash) = (*bash)->next;
		(*bash)->proc = fork();
		if ((*bash)->proc < 0)
			exit(errno);
		else if ((*bash)->proc == 0)
		{
			dup2(tmp->pipe[0], STDIN_FILENO);
			dup2((*bash)->pipe[1], STDOUT_FILENO);
			ft_close_pipe(start);
			ft_execve(bash, envp, line, 1);
		}
		tmp = tmp->next;
	}
	return (tmp);
}

/* -> Funzione helper che controlla l'ultima pipe presente e
	  attende l'uscita dei vari processi, salvando l'exit status <- */
void	ft_pipe_help(t_bash **bash, t_bash **start, char **envp, char *line)
{
	int		status;
	t_bash	*tmp;

	tmp = ft_pipe_help_b(bash, start, envp, line);
	status = 0;
	*bash = (*bash)->next;
	(*bash)->proc = fork();
	if ((*bash)->proc < 0)
		exit(errno);
	else if ((*bash)->proc == 0)
	{
		dup2(tmp->pipe[0], STDIN_FILENO);
		ft_close_pipe(start);
		ft_execve(bash, envp, line, 1);
	}
	ft_close_pipe(start);
	tmp = (*start);
	while (*start)
	{
		waitpid((*start)->proc, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		*start = (*start)->next;
	}
}

/* -> Quando chiamata esegue i comandi che sono separati dalla Pipe,
	   anche consecutivamente <- */
void	ft_pipe(t_bash **bash, char **envp, char *line)
{
	t_bash	*start;

	start = *bash;
	(*bash)->proc = fork();
	if ((*bash)->proc < 0)
		exit(errno);
	else if ((*bash)->proc == 0)
	{
		dup2((*bash)->pipe[1], STDOUT_FILENO);
		ft_close_pipe(&start);
		ft_execve(bash, envp, line, 1);
	}
	ft_pipe_help(bash, &start, envp, line);
}
