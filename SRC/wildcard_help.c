/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_help.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:47:55 by fdrudi            #+#    #+#             */
/*   Updated: 2022/07/12 13:01:25 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Funzione strncmp modificata per i controlli sulla wildcard <- */
int	wd_strncmp(char **s1, char *s2, int *pos, int j)
{
	int	i;

	i = 0;
	while (s1[j][i] != '\0')
	{
		if (s1[j][i] != s2[(*pos)])
			return (0);
		if (s1[j + 1] == NULL)
		{
			if (s1[j][i + 1] == '\0' && s2[(*pos) + 1] != '\0')
			{
				(*pos)++;
				while (s1[j][i] != s2[(*pos)])
					if (s2[(*pos)++] == '\0')
						return (0);
			}
		}
		if (s1[j][i] == '\0')
			return (1);
		i++;
		(*pos)++;
	}
	return (1);
}

/*
	tutto ció che leggi qui doveva trovarsi all'interno del ciclo
	a riga 62 di questo file. Maledetta norma!
*/
int	wd_check(char **wild, char *name, int *i, int *j)
{
	while (wild[(*i)] != NULL && wild[(*i)][0] == '*')
		(*i)++;
	if (wild[(*i)] != NULL)
	{
		while (wild[(*i)][0] != name[(*j)])
		{
			if (name[(*j)] == '\0')
				return (0);
			(*j)++;
		}
		if (wild[(*i) + 1] == NULL)
			return (wd_strncmp(wild, name, j, *i));
	}
	else
		return (1);
	return (-1);
}

/* -> Funzione helper che esegue i controlli per verificare
	  i file corrispondenti alle richieste della wildcard <- */
int	ft_check_wildcard(char **wild, char *name)
{
	int	i;
	int	j;
	int	check;

	i = 0;
	j = 0;
	check = 0;
	while (wild[i] != NULL)
	{
		while (wild[i][0] == '*')
		{
			check = wd_check(wild, name, &i, &j);
			if (check != -1)
				return (check);
		}
		if (wd_strncmp(wild, name, &j, i) == 0)
			return (0);
		i++;
	}
	return (1);
}
