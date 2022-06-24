/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcerchi <mcerchi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:01:31 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/24 19:37:13 by mcerchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCL/minishell.h"

int	ft_pwd(char **cmd, char **envp)
{
	int		*index;
	char	*tmp;

	if (ft_invalid_option(ft_new_env(cmd, 0)) == 1)
		return (1);
	index = find_it(envp, "PWD");
	tmp = NULL;
	if (index != NULL)
	{
		tmp = ft_substr(envp[index[0]], index[1] + 1,
				ft_strlen(envp[index[0]]) - index[1]);
		printf("%s\n", tmp);
		free(index);
		free(tmp);
	}
	return (0);
}

int	ft_env(char **cmd, char **envp)
{
	int	i;

	i = 0;
	if (ft_invalid_option(ft_new_env(cmd, 0)) == 1)
		return (1);
	while (envp[i])
	{
		if (ft_strchr(envp[i], '=') != 0)
			printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

int	ft_exit(char **cmd, char **envp)
{
	int	i;

	if (cmd[1] && cmd[2])
		return (fd_printf(2, "exit: too many arguments\n"));
	ft_free(envp);
	if (!cmd[1])
	{
		printf("\nExit\n");
		exit(g_exit_status);
	}
	i = -1;
	if (cmd[1][0] == '-')
		i++;
	while (cmd[1][++i])
	{
		if (!ft_isdigit(cmd[1][i]))
		{
			fd_printf(2, "\nExit\nbash: exit: %s: numeric argument required\n",
				cmd[1]);
			exit(255);
		}
	}
	printf("\nExit\n");
	exit(ft_atoi(cmd[1]));
}
