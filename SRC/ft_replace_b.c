/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replace_b.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:55:45 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/21 16:57:43 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Sostituzione della variabile con il suo valore <- */
int	*find_it(char **envp, char *to_find)
{
	int		*index;
	int		i;

	i = 0;
	index = (int *) malloc (sizeof(int) * 3);
	while (i < 3)
		index[i++] = 0;
	while (envp[index[0]])
	{
		while (envp[index[0]][index[1]] == to_find[index[2]])
		{
			index[1]++;
			if (!to_find[index[2] + 1])
			{
				if (envp[index[0]][index[1]] == '=')
					return (index);
				return (NULL);
			}
			index[2]++;
		}
		index[0]++;
		index[1] = 0;
		index[2] = 0;
	}
	return (NULL);
}

/* -> Funzione che unisce 3 stringhe <- */
char	*ft_replace_join(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*dst;

	tmp = ft_strjoin(s1, s2);
	free(s1);
	if (*s2 != '\0')
		free(s2);
	dst = ft_strjoin(tmp, s3);
	free(tmp);
	free(s3);
	return (dst);
}

/* -> Elimina un carattere in posizione 'pos' e
	  ritorna la stringa modificata e riallocata <- */
char	*ft_delete_char(char *s, int pos)
{
	char	*s1;
	char	*s2;
	char	*dst;

	s1 = ft_substr(s, 0, pos);
	if (s[pos + 1] == '\0')
	{
		free (s);
		s2 = NULL;
		dst = NULL;
		return (s1);
	}
	s2 = ft_substr(s, pos + 1, ((int) ft_strlen(s) - (pos)));
	dst = ft_strjoin(s1, s2);
	free(s);
	free(s1);
	free(s2);
	return (dst);
}