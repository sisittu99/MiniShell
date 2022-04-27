#include "../INCL/minishell.h"

int	nb_words(char *s, char c)
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
			typequote = *s;
			s++;
			while (s++ != typequote);
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
		if (s[i] == '\'' || s[i] == '\"')
		{
			typequote = s[i];
			i++;
			if (typequote == '\"')
				s = ft_replace(s, envp);
			while (s[i++] != typequote);
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

char	**ms_split(const char *s, char c, char *envp[])
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
