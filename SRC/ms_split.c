/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 17:46:29 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/21 17:46:29 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Conta il numero delle parole all'interno
	  della stringa passata <- */
int	ms_nb_words(char *s, char c)
{
	int		i;
	int		j;
	int		a;

	i = 0;
	j = 0;
	a = 0;
	while (s[a] != '\0')
	{
		if (s[a] == '\'' || s[a] == '\"')
			nb_words_help(s, a, &i, &j);
		nb_words_redir(s, a, &j);
		if ((s[a] != c) && (j == 0))
		{
			j = 1;
			i++;
		}
		if (s[a] == c)
			j = 0;
		a++;
	}
	return (i);
}

/* -> Divide ed assegna le parole calcolate
	  in precedenza all'interno della matrice <- */
void	ms_wds_assign(char *s, char c, char **dest, int len)
{
	int	i[2];
	int	x;

	i[0] = 0;
	x = 0;
	i[1] = -1;
	while (i[0] <= len)
	{
		if ((s[i[0]] == '\'' || s[i[0]] == '\"'))
			wds_assign_help(s, i, len);
		if (s[i[0]] != c && i[1] < 0)
			i[1] = i[0];
		else if ((s[i[0]] == c || i[0] == len) && i[1] >= 0)
		{
			dest[x++] = ft_substr(s, i[1], (i[0] - i[1]));
			i[1] = -1;
		}
		if (i[0] > 0)
			wds_assign_help_b(s, i, dest, &x);
		i[0]++;
	}
	dest[x] = 0;
}

char	**ms_split(char *s)
{
	char	**dest;
	char	*tmp;
	char	c;
	size_t	len;

	if (!s)
		return (NULL);
	c = ' ';
	tmp = ft_strdup(s);
	len = (ft_strlen(tmp));
	dest = (char **) malloc ((ms_nb_words(tmp, c) + 1) * sizeof(char *));
	if (!dest)
		return (NULL);
	ms_wds_assign(tmp, c, dest, (int)len);
	return (dest);
}
