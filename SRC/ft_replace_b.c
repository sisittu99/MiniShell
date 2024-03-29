/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replace_b.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcerchi <mcerchi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:55:45 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/30 13:11:32 by mcerchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Sostituzione della variabile con il suo valore <- */
int	*find_it(char **envp, char *to_find)
{
	int		*index;

	index = ft_int_bzero(3);
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
		index[1] = 0;
		index[2] = 0;
		index[0]++;
	}
	free(index);
	return (NULL);
}

/* -> Funzione che unisce 3 stringhe <- */
char	*ft_replace_join(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*dst;

	tmp = ft_strjoin(s1, s2);
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	dst = ft_strjoin(tmp, s3);
	if (tmp)
		free(tmp);
	if (s3)
		free(s3);
	return (dst);
}

/* -> Elimina un carattere in posizione 'pos' e
	  ritorna la stringa modificata e riallocata <- */
void	ft_delete_char(char **s, int pos)
{
	char	*s1;
	char	*s2;

	s1 = ft_substr(*s, 0, pos);
	if ((*s)[pos + 1] == '\0')
	{
		free(*s);
		s2 = NULL;
		*s = ft_strdup(s1);
		free(s1);
		return ;
	}
	s2 = ft_substr(*s, pos + 1, ((int) ft_strlen(*s) - (pos)));
	free(*s);
	*s = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return ;
}

void	ft_init_replace(char **s1, char **s2, char **s3)
{
	(*s1) = NULL;
	(*s2) = NULL;
	(*s3) = NULL;
}

void	ft_init_arr_replace(int *i, int pos, char *tmp)
{
	i[0] = 0;
	i[2] = pos;
	i[1] = ft_check_var(tmp, pos + 1);
}
