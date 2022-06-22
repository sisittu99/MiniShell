/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:26:38 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/21 16:26:38 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Funzione helper che controlla le priorità degli && e || <- */
int	ft_check_exec_help(t_bash **tmp, char **envp, char *line, int *def)
{
	if (ft_and_or(tmp, envp, ft_strjoin(line, "\n"), def) == 0)
	{
		if ((*tmp)->next != NULL && (*tmp)->next->par != 0)
		{
			*tmp = (*tmp)->next;
			return (1);
		}
	}
	if ((*tmp)->next == NULL)
	{
		(*def) = 0;
		return (0);
	}
	*tmp = (*tmp)->next;
	if ((*tmp)->next == NULL)
		ft_lonely_cmd(tmp, envp, ft_strjoin(line, "\n"));
	return (1);
}

/* -> Funzione che controlla l'esecuzione dei comandi
	  e le priorità date dai vari separatori <- */
int	ft_check_exec(t_bash **tmp, char **envp, char *line)
{
	static int	def;

	if (((*tmp)->pipe[0] != 0 && (*tmp)->pipe[1] != 0)
		&& (*tmp)->next != NULL)
	{
		ft_pipe(tmp, envp, ft_strjoin(line, "\n"));
		def = 1;
	}
	if ((*tmp)->sep == '|' || (*tmp)->sep == '&')
		return (ft_check_exec_help(tmp, envp, line, &def));
	else
	{
		def = 0;
		*tmp = (*tmp)->next;
	}
	return (1);
}

/* -> Gestisce l'esecuzione dei comandi, facendo controlli sia sui separatori,
	  che sui redirect, che sulle funzioni Builtin, ecc... <- */
int	ft_execute(t_bash **bash, char **envp, char **line)
{
	t_bash	*tmp;
	int		lvl;

	tmp = *bash;
	lvl = 0;
	ft_check_new_cmd(&tmp, line, envp);
	if ((*bash)->next == NULL)
		return (ft_lonely_cmd(bash, envp, ft_strjoin(*line, "\n")));
	while (tmp)
	{
		if (tmp->par != 0)
		{
			lvl = tmp->par;
			while (tmp)
			{
				if (tmp->par != lvl)
					break ;
				if (ft_check_exec(&tmp, envp, *line) == 0)
					return (0);
			}
		}
		else
			if (ft_check_exec(&tmp, envp, *line) == 0)
				return (0);
	}
	return (1);
}
