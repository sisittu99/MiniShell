#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	char	c1;
	char	c2;

	while (s1 || s2)
	{
		c1 = *s1++;
		c2 = *s2++;
		if (c1 != c2 || c1 == '\0' || c2 == '\0')
			return (0);
	}
	return (1);
}
