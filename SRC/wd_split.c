/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wd_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 12:10:23 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/24 13:10:09 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

static int	nb_words(const char *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (*s != '\0')
	{
		if ((*s != c) && (j == 0))
		{
			j = 1;
			i++;
		}
		if (*s == c)
		{
			j = 0;
			i++;
		}
		s++;
	}
	return (i);
}

static void	wds_assign(const char *s, char c, char **dest, size_t len)
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
char	**wd_split(const char *s, char c)
{
	char	**dest;
	size_t	len;

	if (!s)
		return (NULL);
	len = (ft_strlen(s));
	dest = (char **) malloc (sizeof(char *) * (nb_words(s, c) + 1));
	if (!dest)
		return (NULL);
	wds_assign(s, c, dest, len);
	return (dest);
}
