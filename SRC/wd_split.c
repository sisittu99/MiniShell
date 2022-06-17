#include "../INCL/minishell.h"

static int	nb_words(const char *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (*s != '\0')
	{
		if ((*s != c) && (j == 0))
		{
			j = 1;
			i++;
		}
		if (*s == c)
		{
			j = 0;
			i++;
		}
		s++;
	}
	return (i);
}

static char	**wds_assign(const char *s, char c, char **dest, size_t len)
{
	size_t	i;
	size_t	x;
	int		j;

	i = 0;
	x = 0;
	j = -1;
	while (i <= len)
	{
		if (s[i] != c && j < 0)
			j = i;
		else if ((s[i] == c || i == len) && j >= 0)
		{
			dest[x++] = ft_strdup("*");
			dest[x++] = ft_substr(s, j, (i - j));
			if (s[i] == c && s[i + 1] == '\0')
				dest[x++] = ft_strdup("*");
			j = -1;
		}
		i++;
	}
	dest[x] = NULL;
	i = 0;
	while (dest[i] != NULL)
	{
		printf("%s\n", dest[i]);
		i++;
	}
	return (dest);
}

char	**wd_split(const char *s, char c)
{
	char	**dest;
	size_t	len;

	if (!s)
		return (NULL);
	len = (ft_strlen(s));
	dest = (char **) malloc (sizeof(char *) * (nb_words(s, c) + 1));
	if (!dest)
		return (NULL);
	wds_assign(s, c, dest, len);
	return (dest);
}
