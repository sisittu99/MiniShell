/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 18:36:22 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/24 15:42:57 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*cp;

	if (n == 0)
		return ;
	i = -1;
	cp = s;
	while (++i < n)
		cp[i] = '\0';
}

int	*ft_int_bzero(int n)
{
	int	i;
	int	*cp;

	if (n == 0)
		return (NULL);
	i = -1;
	cp = (int *) malloc (sizeof(int) * n + 1);
	if (!cp)
		return (NULL);
	while (++i < n)
		cp[i] = 0;
	return (cp);
}
