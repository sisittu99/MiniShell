/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_and_or.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:11:03 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/21 16:32:04 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Funzione helper che ritorna la condizione per || <- */
int	ft_or(t_bash **bash, int *def, int *par)
{
	if (g_exit_status != 0)
	{
		(*def) = 0;
		(*par) = (*bash)->par;
		return (1);
	}
	(*par) = (*bash)->par;
	while ((*bash)->next != NULL
		&& (((*bash)->sep == '|' && (*bash)->par == (*bash)->next->par)
			|| ((*bash)->next->par >= (*par))))
		*bash = (*bash)->next;
	(*def) = 1;
	(*par) = (*bash)->par;
	return (0);
}

/* -> Funzione helper che ritorna la condizione per && <- */
int	ft_and(t_bash **bash, int *def, int *par)
{
	if (g_exit_status != 0)
	{
		(*par) = (*bash)->par;
		while ((*bash)->next != NULL
			&& (((*bash)->sep == '&'
					&& (*bash)->par == (*bash)->next->par)
				|| ((*bash)->next->par >= (*par))))
			*bash = (*bash)->next;
		(*def) = 1;
		(*par) = (*bash)->par;
		return (0);
	}
	(*def) = 0;
	(*par) = (*bash)->par;
	return (1);
}

/* -> Funzione che controlla e ritorna la condizione per && o ||
	  controllando quando eseguire o meno <- */
int	ft_and_or(t_bash **bash, char **envp, char *line, int *def)
{
	static int	par;

	if ((*def) == 0)
		ft_lonely_cmd(bash, envp, line);
	if ((*bash)->sep == '&')
		return (ft_and(bash, def, &par));
	else if ((*bash)->sep == '|')
		return (ft_or(bash, def, &par));
	return (1);
}
