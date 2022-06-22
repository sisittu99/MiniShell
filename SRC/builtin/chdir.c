/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:20:43 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/21 16:20:43 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCL/minishell.h"

/*
	Gestisce le variabili PWD e OLDPWD durante l'uso di cd.
*/
void	ft_handle_pwd_var(t_bash **bash, char **envp, int casus)
{
	char	*new_pwd;
	int		*index;

	new_pwd = (char *) malloc (sizeof(char) * 256);
	getcwd(new_pwd, 256);
	if (casus == 0)
	{
		index = find_it(envp, "OLDPWD");
		ft_env_var_found(ft_strjoin("OLDPWD=", new_pwd), bash, index);
	}
	else
	{
		index = find_it(envp, "PWD");
		ft_env_var_found(ft_strjoin("PWD=", new_pwd), bash, index);
	}
	free(new_pwd);
	free(index);
}

/*
	In caso di nessun argomento, cerca la var HOME
	e sposta il processo in quella cartella.
*/
char	*ft_find_home(char **envp)
{
	char	*dir;
	int		*index;

	index = find_it(envp, "HOME");
	dir = NULL;
	if (index != NULL)
		dir = ft_substr(envp[index[0]], index[1] + 1,
				ft_strlen(envp[index[0]]));
	if (index)
		free(index);
	return (dir);
}

int	ft_cd(t_bash **bash, char **cmd, char **envp)
{
	char	*dir;
	int		res;

	dir = NULL;
	if (ft_invalid_option(cmd) == 1)
		return (1);
	if (cmd[1] == NULL)
		dir = ft_find_home(envp);
	else
		dir = ft_strdup(cmd[1]);
	if (((*bash)->pipe[0] == 0 && (*bash)->pipe[1] == 0)
		&& (*bash)->next == NULL)
		(*bash)->envp = ft_new_env(envp, 0);
	{
		ft_handle_pwd_var(bash, envp, 0);
		res = chdir(dir);
		free(dir);
		if (res == -1)
		{
			fd_printf(2, "bash: cd: %s: No such file or directory\n", dir);
			return (1);
		}
		ft_handle_pwd_var(bash, envp, 1);
	}
	return (0);
}
