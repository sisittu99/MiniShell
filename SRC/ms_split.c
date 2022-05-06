#include "../INCL/minishell.h"

int	ms_strchr(char *s, int pos, char c)
{
	while (s[pos] != '\0')
	{
		if (s[pos] == c)
			return (pos);
		pos++;
	}
	return (-1);
}

/* -> Controlla che nel conteggio delle parole, se presenti,
	  le '\'' e le '\"' siano contate come parola singola <- */
void	nb_words_help(char *s, int *i, int *j)
{
	char	typequote;

	typequote = *s;
	s++;
	if (ms_strchr(s, *i, typequote) > -1)
	{
		while (*s != typequote)
			s++;
		if (*j == 0)
		{
			*j = 1;
			*i += 1;
		}
	}
}

/* -> Conta il numero delle parole all'interno
	  della stringa passata <- */
static int	nb_words(char *s, char c)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (*s != '\0')
	{
		if (*s == '\'' || *s == '\"')
			nb_words_help(s, &i, &j);
		if ((*s != c) && (j == 0))
		{
			j = 1;
			i++;
		}
		if (*s == c)
			j = 0;
		s++;
	}
	return (i);
}

/* -> Controlla che nell'assegnazione delle parole, se presenti,
	  le '\'' e le '\"' siano contate come parola singola <- */
char	*wds_assign_help(char *s, int *i, int *j, int len)
{
	int		a;
	char	typequote;

	typequote = s[*i];
	a = *i;
	*i += 1;
	if (ms_strchr(s, *i, typequote) > -1)
	{
		while (s[*i] != typequote)
			*i += 1;
		s = ft_delete_char(s, a);
		s = ft_delete_char(s, *i - 1);
		len -= 1;
	}
	if (*j < 0)
		*j = a;
	return (s);
}

/* -> Divide ed assegna le parole calcolate
	  in precedenza all'interno della matrice <- */
void	wds_assign(char *s, char c, char **dest, size_t len)
{
	size_t	i;
	size_t	x;
	int		j;

	i = 0;
	x = 0;
	j = -1;
	while (i <= len)
	{
		if ((s[i] == '\'' || s[i] == '\"'))
			s = wds_assign_help(s, (int *)&i, &j, len);
		if (s[i] != c && j < 0)
			j = i;
		else if ((s[i] == c || i == len) && j >= 0)
		{
			dest[x++] = ft_substr(s, j, (i - j));
			j = -1;
		}
		i++;
	}
	dest[x] = 0;
}

char	**ms_split(char *s)
{
	char	**dest;
	char	*tmp;
	char	c = ' ';
	size_t	len;

	if (!s)
		return (NULL);
	tmp = ft_strdup(s);
	len = (ft_strlen(tmp));
	dest = (char **) malloc ((nb_words(tmp, c) + 1) * sizeof(char *));
	if (!dest)
		return (NULL);
	wds_assign(tmp, c, dest, len);
	return (dest);
}
