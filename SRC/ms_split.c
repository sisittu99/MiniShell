#include "../INCL/minishell.h"

static int	nb_words(char *s, char c)
{
	int		i;
	int		j;
	char	typequote;

	i = 0;
	j = 0;
	while (*s != '\0')
	{
		if (*s == '\'' || *s == '\"')
		{
			typequote = *s;								//RICERCA DEGLI APICI O VIRGOLETTE E
			s++;										//CONTEGGIO COME STRINGA SINGOLA/PAROLA
			while (*s != typequote)
				s++;
			if (j == 0)
			{
				j = 1;
				i++;
			}
		}
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

static char	**wds_assign(char *s, char c, char **dest, size_t len, char *envp[])
{
	size_t	i;
	size_t	x;
	int		j;
	char	typequote;

	i = 0;
	x = 0;
	j = -1;
	while (i <= len)
	{
		if ((s[i] == '\'' || s[i] == '\"'))
		{
			typequote = s[i];
			j = i;
			i++;
			if (typequote == '\"')					//CONTROLLO SE SIAMO DENTRO LE VIRGOLETTE
				s = ft_replace(s, envp);			//CONTROLLO E REPLACE DELLE VARIABILI
			while (s[i] != typequote && s[i] != '\0')
			{
				if (s[i + 1] == typequote)
				{
					s = ft_delete_char(s, j);				//RICERCA DEGLI APICI O VIRGOLETTE
					s = ft_delete_char(s, i);
					i--;				//AGGIUNTA STRINGA/PAROLA ALLA MATRICE
					len -= 2;
					break;
				}
				i++;
			}
		}
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
	return (dest);
}

char	**ms_split(char *s, char c, char *envp[])
{
	char	**dest;
	size_t	len;

	if (!s)
		return (NULL);
	len = (ft_strlen(s));
	dest = (char **) malloc ((nb_words(s, c) + 1) * sizeof(char *));
	if (!dest)
		return (NULL);
	wds_assign(s, c, dest, len, envp);
	return (dest);
}
