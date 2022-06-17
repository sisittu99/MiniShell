#include "../INCL/minishell.h"

/* -> Sostituzione della variabile con il suo valore <- */
int	*find_it(char **envp, char *to_find)
{
	int		*index;
	int		i;

	i = 0;
	index =(int *) malloc (sizeof(int) * 3);
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

/* -> Controlla che la presunta variabile sia scritta
	  secondo le regole grammaticali corrette <- */
int	ft_check_var(char *s, int pos)
{
	int	i;

	i = 0;
	if (s[pos] == 63 && (s[pos + 1] == 32 || s[pos + 1] == '\0'))
		return (-1);
	if (ft_isalpha(s[pos]) || s[pos] == 95)
	{
		while (s[pos + i] != '\0' && (ft_isalpha(s[pos + i]) || ft_isdigit(s[pos + i])
			|| s[pos + i] == 95))
			i++;
		return (i);
	}
	return (0);
}

char	*ft_find_wildcard(char *s, int *pos)
{
	int		i;
	char	*tmp;

	while (*pos > 0 && s[*pos] != ' ')
		*pos--;
	if (*pos < 0)
		pos = 0;
	i = *pos + 1;
	while (s[i] != ' ' && s[i] != '\0')
		i++;
	tmp = ft_substr(s, *pos, (i - *pos));
	return (tmp);
}

char	*ft_wildcard(char *s, char **envp, int pos, int *ret_i)
{
	char			*s1;
	char			*s2;
	char			**wild;
	char			*pwd;
	DIR				*dir;
	struct dirent	*rdir;

	// pwd = (char *) malloc (sizeof(char) * 256);
	// getcwd(pwd, 256);
	// dir = opendir(pwd);
	// if (dir == NULL)
	// {
	// 	fd_printf(2, "error: could not open dir\n");
	// 	return (NULL);
	// }
	wild = wd_split(ft_find_wildcard(s, &pos), '*');
	exit(0);
	s1 = ft_substr(s, 0, pos);
	rdir = readdir(dir);
	// while (rdir != NULL)
	// {

	// }
}

char	*ft_replace_tilde(char *s, char **envp, int pos, int *ret_i)
{
	char	*s1;
	char	*s2;
	char	*s3;
	int		*index;
	int		i;

	s1 = ft_substr(s, 0, pos);
	index = find_it(envp, "HOME");
	i = pos + 1;
	if (index != NULL)
	{
		s2 = ft_substr(envp[index[0]], index[1] + 1,
				ft_strlen(envp[index[0]]));
		*ret_i = pos + ft_strlen(s2);
	}
	else
		s2 = ft_strdup("~");
	s3 = ft_substr(s, i, (ft_strlen(s) - i));
	free(s);
	s = ft_replace_join(s1, s2, s3);
	return (s);
}

char	*ft_replace_help(char *s, char **envp, int pos, int *ret_i)
{
	char	*s1;
	char	*s2;
	char	*s3;
	char	*var;
	int		*index;
	int		i;
	int		j;

	j = ft_check_var(s, pos + 1);
	if (j > 0 || j == -1)
	{
		s1 = ft_substr(s, 0, pos);
		if (j == -1)
		{
			s2 = ft_itoa(exit_status);
			i = pos + 1 + 1;
		}
		else if (j > 0)
		{
			var = ft_substr(s, (pos + 1), j);
			i = pos + 1 + j;
			index = find_it(envp, var);
			if (index != NULL)
			{
				s2 = ft_substr(envp[index[0]], index[1] + 1,
						ft_strlen(envp[index[0]]));
			}
			else
				s2 = ft_strjoin("$", var);
			free(var);
		}
		*ret_i = pos + ft_strlen(s2);
		s3 = ft_substr(s, i, (ft_strlen(s) - i));
		free(s);
		s = ft_replace_join(s1, s2, s3);
	}
	return (s);
}

/* -> Controllo e ricerca della variabile all'interno della srtinga.
      Se la variabile è presente, suddivisione della stringa in 3 e
	  sostituzione della variabile con il suo valore.
	  || ** Questa funzione dev'essere chiamata dopo il controllo e la
	  conferma che la variabile dev'essere sostituita col valore ** || <- */
void	ft_replace(char **s, char **envp, int pos, int *ret_i)
{
	char	*tmp;

	tmp = NULL;
	if ((*s)[pos] == '*')
	{
		tmp = ft_strdup(*s);
		free(*s);
		(*s) = ft_wildcard(tmp, envp, pos, ret_i);
	}
	else if ((*s)[pos] == '~')
	{
		tmp = ft_strdup(*s);
		free(*s);
		(*s) = ft_replace_tilde(tmp, envp, pos, ret_i);
	}
	else if ((*s)[pos + 1] == '\0')
		return ;
	else if ((*s)[pos] == '$' && ((*s)[pos + 1] != '\"'))
	{
		tmp = ft_strdup(*s);
		free(*s);
		(*s) = ft_replace_help(tmp, envp, pos, ret_i);
	}
	return ;
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
