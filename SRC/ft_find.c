/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcerchi <mcerchi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:14:37 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/30 18:42:07 by mcerchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Funzione helper per la sostituzione della tilde o wildcard <- */
void	ft_find_tilde_help_b(int *pos_apex, int i, char *line)
{
	if (pos_apex[0] < pos_apex[1])
	{
		pos_apex[0] = ms_strchr(line, i, '\'');
		pos_apex[1] = ms_strchr(line, (pos_apex[0] + 1), '\'');
	}
	if (pos_apex[2] < pos_apex[3])
	{
		pos_apex[2] = ms_strchr(line, i, '\"');
		pos_apex[3] = ms_strchr(line, (pos_apex[2] + 1), '\"');
	}
}

/* -> Funzione helper per la sostituzione della tilde o wildcard <- */
void	ft_find_tilde_help(int *pos_apex, int *i, int pos)
{
	if ((pos_apex[0] < pos && pos < pos_apex[1])
		&& (pos_apex[2] < pos && pos < pos_apex[3]))
	{
		if (pos_apex[1] > pos_apex[3])
			(*i) = pos_apex[1] + 1;
		else
			pos_apex[3] += 1;
	}
	else if (pos_apex[0] < pos && pos < pos_apex[1])
		(*i) = pos_apex[1] + 1;
	else if (pos_apex[2] < pos && pos < pos_apex[3])
		(*i) = pos_apex[3] + 1;
	else
		(*i)++;
}

/* -> Funzione che cerca un carattere passato facendo
	  il replace col suo valore <- */
void	ft_find_tilde(char **line, char **envp, char re_dir, char c)
{
	int	pos;
	int	pos_apex[4];
	int	i;

	i = 0;
	pos_apex[0] = 0;
	pos_apex[1] = 1;
	pos_apex[2] = 0;
	pos_apex[3] = 1;
	pos = ms_strchr(*line, i, c);
	while (pos != -1)
	{
		ft_find_tilde_help_b(pos_apex, i, *line);
		if (!(pos_apex[0] < pos && pos < pos_apex[1])
			&& !(pos_apex[2] < pos && pos < pos_apex[3]) && re_dir != '1')
		{
			ft_replace(line, envp, pos, &i);
			i++;
		}
		else
			ft_find_tilde_help(pos_apex, &i, pos);
		pos = ms_strchr(*line, i, c);
	}
}
