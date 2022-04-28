#include "../INCL/minishell.h"

/* -> Sostituzione della variabile con il suo valore
*/
char	*find_it(char **envp, char *to_find)
{
	int		index[3];
	int		i;

	i = 0;
	while (i < 3)
		index[i++] = 0;
	while (envp[index[0]])
	{
		while (envp[index[0]][index[1]] == to_find[index[2]])
		{
			index[1]++;
			if (!to_find[index[2] + 1])
				return (ft_substr(envp[index[0]], ++index[1],
						ft_strlen(envp[index[0]])));
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
	free(s2);
	dst = ft_strjoin(tmp, s3);
	free(tmp);
	free(s3);
	return (dst);
}

/* -> Controllo e ricerca della variabile all'interno della srtinga.
      Se la variabile è presente, suddivisione della stringa in 3 e
	  sostituzione della variabile con il suo valore.
	  || ** Questa funzione dev'essere chiamata dopo il controllo e la
	  	 conferma che la variabile dev'essere sostituita col valore ** ||
*/
char	*ft_replace(char *s, char *envp[])
{
	char	*s1;
	char	*s2;
	char	*s3;
	char	*var;

	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[j] != '$')
		j++;
	if (j == (int)ft_strlen(s))
		return (s);
	s1 = ft_substr(s, 0, j);
	i = j;
	while (s[i] >= 65 || s[i] <= 90 || s[i] == 95)
		i++;
	var = ft_substr(s, j, i - j);
	s2 = find_it(envp, var);
	free(var);
	s3 = ft_substr(s, i, (ft_strlen(s) - i));
	free(s);
	return (ft_replace_join(s1, s2, s3));
}

char	*ft_delete_char(char *s, int pos)
{
	char	*s1;
	char	*s2;
	char	*dst;

	printf("s : %s$\n", s);
	s1 = ft_substr(s, 0, pos);
	printf("s1 : %s$\n", s1);
	if (s[pos + 1] == '\0')
	{
		free (s);
		s2 = NULL;
		dst = NULL;
		return (s1);
	}
	s2 = ft_substr(s, pos + 1, ((int) ft_strlen(s) - (pos)));
	printf("s2 : %s$\n", s2);
	dst = ft_strjoin(s1, s2);
	printf("dst : %s$\n\n", dst);
	free(s);
	free(s1);
	free(s2);
	return (dst);
}
