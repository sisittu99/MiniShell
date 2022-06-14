/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 18:02:33 by fdrudi            #+#    #+#             */
/*   Updated: 2022/02/02 18:02:33 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_putchar(char c, int fd)
{
	return (write(fd, &c, 1));
}

int	ft_putstr(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		s = "(null)";
	while (*s != '\0')
	{
		i += ft_putchar(*s, fd);
		s++;
	}
	return (i);
}

int	ft_putnbr(int n, int fd)
{
	int	i;

	i = 0;
	if (n == -2147483648)
	{
		i += ft_putchar('-', fd);
		i += ft_putchar('2', fd);
		n = 147483648;
	}
	if (n < 0)
	{
		i += ft_putchar('-', fd);
		n *= -1;
	}
	if (n >= 10)
	{
		i += ft_putnbr(n / 10, fd);
		i += ft_putnbr(n % 10, fd);
	}
	else
	{
		i += ft_putchar(48 + n, fd);
	}
	return (i);
}
