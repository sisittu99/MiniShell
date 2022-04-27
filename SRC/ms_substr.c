#include "../INCL/minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dest;
	size_t	i;

	if (!s)
		return (NULL);
	if (ft_strlen(s) <= start)
		start = ft_strlen(s);
	if (start + len >= ft_strlen(s))
		len = ft_strlen(s) - start;
	i = 0;
	dest = (char *) malloc (sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
	while (len--)
		dest[i++] = s[start++];
	dest[i] = '\0';
	return (dest);
}

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
