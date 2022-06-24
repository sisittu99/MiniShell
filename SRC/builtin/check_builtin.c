/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:24:20 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/24 13:12:34 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCL/minishell.h"

void	ft_exec_builtin(t_bash **bash, char **envp, int def)
{
	if ((*bash)->built == 0)
		g_exit_status = ft_echo((*bash)->cmd);
	else if ((*bash)->built == 1)
		g_exit_status = ft_cd(bash, (*bash)->cmd, envp);
	else if ((*bash)->built == 2)
		g_exit_status = ft_pwd((*bash)->cmd, envp);
	else if ((*bash)->built == 3)
		g_exit_status = ft_export(bash, (*bash)->cmd, envp);
	else if ((*bash)->built == 4)
		g_exit_status = ft_unset(bash, (*bash)->cmd, envp);
	else if ((*bash)->built == 5)
		g_exit_status = ft_env((*bash)->cmd, envp);
	else if ((*bash)->built == 6)
		ft_exit((*bash)->cmd, envp);
	if (def == 1)
		exit(g_exit_status);
	return ;
}

/* Crea la matrice con la quale controllare che il comando sia una built-in,
	scritta correttamente */
char	**ft_builtin_assgn(void)
{
	char	**built;

	built = (char **) malloc (sizeof(char *) * 8);
	if (!built)
		exit(errno);
	built[0] = ft_strdup("echo");
	built[1] = ft_strdup("cd");
	built[2] = ft_strdup("pwd");
	built[3] = ft_strdup("export");
	built[4] = ft_strdup("unset");
	built[5] = ft_strdup("env");
	built[6] = ft_strdup("exit");
	built[7] = 0;
	return (built);
}

/* Controlla che il comando sia una built-in.
	In caso positivo, ritorna la pos nella matrice built
	per poter proseguire ed eseguire la funzione corretta. */
int	ft_check_builtin(char *cmd)
{
	int		i;
	char	**built;

	built = ft_builtin_assgn();
	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(cmd, built[i]) == 1)
		{
			ft_free(built);
			return (i);
		}
		i++;
	}
	ft_free(built);
	return (-1);
}

int	ft_invalid_option(char **cmd)
{
	if (cmd[1] != NULL)
	{
		if (cmd[1][0] == '-')
		{
			fd_printf(2, "bash: %s: %s: invalid option\n", cmd[0], cmd[1]);
			ft_free(cmd);
			return (1);
		}
	}
	ft_free(cmd);
	return (0);
}
