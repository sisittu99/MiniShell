/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:47:37 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/30 17:58:8 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Funzione helper che trova la wildcard completa e la ritorna <- */
char	*ft_find_wildcard(char *s, int *pos)
{
	int		i;
	char	*tmp;

	while ((*pos) > 0 && s[(*pos)] != ' ')
		(*pos)--;
	if ((*pos) < 0)
	{
		(*pos) = 0;
		i = (*pos);
	}
	(*pos) += 1;
	i = (*pos);
	while (s[i] != ' ' && s[i] != '\0')
		i++;
	tmp = ft_substr(s, (*pos), (i - (*pos)));
	return (tmp);
}

void	wd_join(char **s1, char **s2, struct dirent *rdir)
{
	if (ft_strchr(rdir->d_name, ' ') != NULL)
	{
		(*s2) = ft_strjoin(*s1, "\'");
		free(*s1);
		(*s1) = ft_strjoin(*s2, rdir->d_name);
		free(*s2);
		(*s2) = ft_strjoin(*s1, "\'");
	}
	else
		(*s2) = ft_strjoin(*s1, rdir->d_name);
	free(*s1);
	*s1 = ft_strjoin(*s2, " ");
	free(*s2);
}

/*
	Legge la cartella e manda a controllare la wildcard.
*/
int	ft_read_dir(char *pwd, char **wild, char **s1, char **s2)
{
	struct dirent	*rdir;
	DIR				*dir;

	dir = opendir(pwd);
	if (dir == NULL)
	{
		fd_printf(2, "bash: error: could not open dir\n");
		return (0);
	}
	rdir = readdir(dir);
	while (rdir != NULL)
	{
		if (ft_check_wildcard(wild, rdir->d_name) == 1)
			wd_join(s1, s2, rdir);
		rdir = readdir(dir);
	}
	closedir(dir);
	*s2 = ft_strtrim(*s1, " ");
	free(*s1);
	*s1 = ft_strdup(*s2);
	free(*s2);
	return (1);
}

char	*ft_join_n_return(char **s1, char **s2, char **s3, char ***wild)
{
	*s2 = ft_strjoin(*s1, *s3);
	free(*s1);
	free(*s3);
	ft_free(*wild);
	return (*s2);
}

/* -> Funzione che sostituisce la wildcard con
	  tutti i file corrispondenti alle caratteristiche <- */
char	*ft_wildcard(char *s, char *pwd, int pos, int *ret_i)
{
	char			*s1;
	char			*s2;
	char			*s3;
	char			**wild;

	ft_init_replace(&s1, &s2, &s3);
	wild = wd_split(ft_find_wildcard(s, &pos), '*');
	s1 = ft_substr(s, 0, pos);
	if (ft_read_dir(pwd, wild, &s1, &s2) == 0)
	{
		ft_free(wild);
		free(s1);
		return (NULL);
	}
	*ret_i = ft_strlen(s1);
	while (s[pos] != ' ' && s[pos] != '\0')
		pos++;
	if (s[pos] != '\0')
	{
		s3 = ft_substr(s, pos, (ft_strlen(s) - pos));
		return (ft_join_n_return(&s1, &s2, &s3, &wild));
	}
	ft_free(wild);
	return (s1);
}
