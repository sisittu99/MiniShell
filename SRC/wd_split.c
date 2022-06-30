/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wd_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcerchi <mcerchi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 12:10:23 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/30 18:41:51 by mcerchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

int	wd_words(char *s, char c)
{
	int	i;
	int	j;
	int	a;

	i = 0;
	j = 0;
	a = 0;
	while (s[a] != '\0')
	{
		if ((s[a] != c) && (j == 0))
		{
			j = 1;
			i++;
		}
		if (s[a] == c)
		{
			j = 0;
			i++;
		}
		a++;
	}
	return (i);
}

void	wd_wds_assign(char *s, char c, char **dest, size_t len)
{
	size_t	i;
	size_t	x;
	int		j;

	i = 0;
	x = 0;
	j = -1;
	while (i <= len)
	{
		if (s[i] != c && j < 0)
			j = i;
		else if (s[i] == c && j < 0)
			dest[x++] = ft_strdup("*");
		else if ((s[i] == c || i == len) && j >= 0)
		{
			dest[x++] = ft_substr(s, j, (i - j));
			if (s[i] == c)
				dest[x++] = ft_strdup("*");
			j = -1;
		}
		i++;
	}
	dest[x] = NULL;
}

/* -> Funzione split modificata per la wildcard.
	  Suddivide tutte le componenti della wildcard
	  mantenendo anche il carattere passatole
	  in una stringa separata dalle altre <- */
char	**wd_split(char *s, char c)
{
	char	**dest;
	size_t	len;

	if (!s)
		return (NULL);
	len = (ft_strlen(s));
	dest = (char **) malloc (sizeof(char *) * (wd_words(s, c) + 1));
	if (!dest)
		return (NULL);
	wd_wds_assign(s, c, dest, len);
	free(s);
	return (dest);
}
