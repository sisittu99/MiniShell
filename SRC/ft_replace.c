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
			{
				if (envp[index[0]][index[1]] == '=')
					return (ft_substr(envp[index[0]], index[1] + 1,
						ft_strlen(envp[index[0]])));
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

int	ft_check_var(char *s, int pos)
{
	int	i;

	i = 0;
	if (s[pos] == 63)
		return (-1);
	if (ft_isalpha(s[pos]) || s[pos] == 95)
	{
		i++;
		while (ft_isalpha(s[pos + i]) || ft_isdigit(s[pos + i]) || s[pos + i] == 95)
			i++;
		return (i);
	}
	return (0);
}

char	*ft_replace(char *s, char *envp[], int pos, int *ret)
{
	char	*s1;
	char	*s2;
	char	*s3;
	char	*var;

	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[pos] != '\"')
	{
		if (s[pos] == '$' && (s[pos + 1] != '\"'))
		{
			j = ft_check_var(s, pos + 1);
			if (j > 0)
			{
				s1 = ft_substr(s, 0, pos);
				printf("Replace s1: %s\t", s1); fflush(stdout);
				var = ft_substr(s, pos + 1, j);
				printf("Replace var: %s\t", var);fflush(stdout);
				s2 = find_it(envp, var);
				printf("Replace s2: %s\t", s2);fflush(stdout);
				free(var);
				i = pos + 1 + j;
				s3 = ft_substr(s, i, (ft_strlen(s) - i));
				printf("Replace s3: %s\t", s3);fflush(stdout);
				free(s);
				s = ft_replace_join(s1, s2, s3);
			}
			else if (j == -1)
				return (NULL); // ** DA AGGIUNGERE REGOLA PER '$?' QUANDO SARÀ TESTATO ** //
		}
		else if (s[pos + 1] == '\0')
			return (s);
		pos++;
	}
	*ret = pos - 1;
	return (s);
}

char	*ft_delete_char(char *s, int pos)
{
	char	*s1;
	char	*s2;
	char	*dst;

	printf("Delete s : %s$\n", s);
	s1 = ft_substr(s, 0, pos);
	printf("Delete s1 : %s$\n", s1);
	if (s[pos + 1] == '\0')
	{
		free (s);
		s2 = NULL;
		dst = NULL;
		return (s1);
	}
	s2 = ft_substr(s, pos + 1, ((int) ft_strlen(s) - (pos)));
	printf("Delete s2 : %s$\n", s2);
	dst = ft_strjoin(s1, s2);
	printf("Delete dst : %s$\n\n", dst);
	free(s);
	free(s1);
	free(s2);
	return (dst);
}
