/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 17:44:38 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/24 17:20:51 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

int	ms_strchr(char *s, int pos, char c)
{
	while (s[pos] != '\0')
	{
		if (s[pos] == c)
			return (pos);
		pos++;
	}
	return (-1);
}

/* -> Controlla che nel conteggio delle parole, se presenti,
	  le '\'' e le '\"' siano contate come parola singola <- */
void	nb_words_help(char *s, int a, int *i, int *j)
{
	char	typequote;

	typequote = s[a];
	a++;
	if (ms_strchr(s, a, typequote) > -1)
	{
		while (s[a] != typequote)
			a++;
		if (*j == 0)
		{
			*j = 1;
			*i += 1;
		}
	}
}

/* -> Controlla se sono presenti redirect nella stringa
	  e in caso non siano separati da spazi dalle altre parole,
	  li separa e li conta <- */
void	nb_words_redir(char *s, int a, int *j)
{
	if (a > 0)
	{
		if ((s[a] == '>' || s[a] == '<') && (s[a - 1] != ' '
				&& s[a - 1] != '>' && s[a - 1] != '<'))
			*j = 0;
		else if ((s[a - 1] == '>' || s[a - 1] == '<')
			&& (s[a] != ' ' && s[a] != '>' && s[a] != '<'))
			*j = 0;
	}
	return ;
}

/* -> Controlla ls presenza di redirect <- */
void	wds_assign_help_b(char *s, int *i, char **dest, int *x)
{
	if ((s[i[0]] == '>' || s[i[0]] == '<') && s[i[0] - 1] != ' '
		&& s[i[0] - 1] != '>' && s[i[0] - 1] != '<')
	{
		if (i[1] < i[0])
			dest[(*x)++] = ft_substr(s, i[1], (i[0] - i[1]));
		i[1] = i[0];
	}
	else if ((s[i[0] - 1] == '>' || s[i[0] - 1] == '<')
		&& (s[i[0]] != ' ' && s[i[0]] != '>' && s[i[0]] != '<'))
	{
		if (i[1] < i[0])
			dest[(*x)++] = ft_substr(s, i[1], (i[0] - i[1]));
		i[1] = i[0];
	}
}

/* -> Controlla che nell'assegnazione delle parole, se presenti,
	  le '\'' e le '\"' siano contate come parola singola <- */
void	wds_assign_help(char *s, int *i, int len)
{
	int		a;
	char	typequote;

	typequote = s[i[0]];
	a = i[0];
	i[0] += 1;
	if (ms_strchr(s, i[0], typequote) > -1)
	{
		while (s[i[0]] != typequote)
			i[0] += 1;
		ft_delete_char(&s, a);
		ft_delete_char(&s, i[0] - 1);
		len -= 1;
	}
	if (i[1] < 0)
		i[1] = a;
	i[0] -= 1;
	return ;
}
