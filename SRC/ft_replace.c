#include "../INCL/minishell.h"

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
	if (j == ft_strlen(s))
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
