/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcerchi <mcerchi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:24:22 by fdrudi            #+#    #+#             */
/*   Updated: 2022/07/09 18:02:51 by mcerchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCL/minishell.h"

int	ft_flag_echo(char **cmd)
{
	int	i;
	int	j;
	int	flag;

	i = 1;
	flag = 0;
	while (cmd[i] && cmd[i][0] == '-' && cmd[i][1] == 'n')
	{
		j = 1;
		while (cmd[i][j])
		{
			if (cmd[i][j] != 'n')
				return (flag);
			j++;
		}
		flag++;
		i++;
	}
	return (flag);
}

int	ft_echo(char **cmd)
{
	int		i;
	int		flag;

	i = 1;
	flag = 0;
	if (cmd[1] != NULL)
	{
		flag = ft_flag_echo(cmd);
		if (cmd[1 + flag])
		{
			while (cmd[i + flag + 1])
				printf("%s ", cmd[(i++) + flag]);
			printf("%s", cmd[i + flag]);
		}
	}
	if (flag == 0)
		printf("\n");
	return (0);
}
