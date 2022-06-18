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
			if (s1[j][i] == '\0' && s2[(*pos)] != '\0')
				return (0);
		}
		if (s1[j][i] == '\0')
			return (1);
		i++;
		(*pos)++;
	}
	return (1);
}

/* -> Funzione helper che esegue i controlli per verificare
	  i file corrispondenti alle richieste della wildcard <- */
int	ft_check_wildcard(char **wild, char *name)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (wild[i] != NULL)
	{
		while (wild[i][0] == '*')
		{
			if (wild[i + 1] != NULL)
			{
				i++;
				while (wild[i][0] != name[j])
				{
					if (name[j] == '\0')
						return (0);
					j++;
				}
			}
			else
				return (1);
		}
		if (wd_strncmp(wild, name, &j, i) == 0)
			return (0);
		j++;
		i++;
	}
	return (1);
}

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

/* -> Funzione che sostituisce la wildcard con
	  tutti i file corrispondenti alle caratteristiche <- */
char	*ft_wildcard(char *s, char *pwd, int pos, int *ret_i)
{
	char			*s1;
	char			*s2;
	char			*s3;
	char			**wild;
	DIR				*dir;
	struct dirent	*rdir;

	s3 = NULL;
	dir = opendir(pwd);
	if (dir == NULL)
	{
		fd_printf(2, "error: could not open dir\n");
		return (NULL);
	}
	wild = wd_split(ft_find_wildcard(s, &pos), '*');
	s1 = ft_substr(s, 0, pos);
	rdir = readdir(dir);
	while (rdir != NULL)
	{
		if (ft_check_wildcard(wild, rdir->d_name) == 1)
		{
			s2 = ft_strjoin(s1, rdir->d_name);
			free(s1);
			s1 = ft_strjoin(s2, " ");
			free(s2);
		}
		rdir = readdir(dir);
	}
	*ret_i = ft_strlen(s1);
	while (s[pos] != ' ' && s[pos] != '\0')
		pos++;
	if (s[pos] != '\0')
	{
		s3 =ft_substr(s, pos, (ft_strlen(s) - pos));
		return (ft_strjoin(s1, s3));
	}
	closedir(dir);
	ft_free(wild);
	return (s1);
}
