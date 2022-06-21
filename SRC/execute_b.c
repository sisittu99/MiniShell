/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_b.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:24:40 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/21 16:26:24 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Funzione Helper che elimina due stringhe all'interno di una matrice <- */
char	**ft_delete_cmd(char **cmd, int pos)
{
	char	**tmp;
	int		i;
	int		j;
	int		len;

	i = 0;
	j = 0;
	len = 0;
	while (cmd[len])
		len++;
	tmp = (char **) malloc (sizeof(char *) * (len - 1));
	while (cmd[j] != NULL)
	{
		if (pos == j)
			j += 2;
		if (cmd[j] != NULL)
		{
			tmp[i] = ft_strdup(cmd[j]);
			i++;
			j++;
		}
	}
	tmp[len - 2] = 0;
	ft_free(cmd);
	return (tmp);
}

/* -> Controlla i redirect presenti ed esegue il comando <- */
void	ft_execve(t_bash **bash, char **envp, char *line, int def)
{
	int	i;

	i = 0;
	if ((*bash)->re_dir)
	{
		while ((*bash)->cmd[i] != 0)
		{
			if (ft_check_re_dir(bash, i, line) == 0)
				i++;
		}
	}
	if ((*bash)->built == -2)
		(*bash)->built = ft_check_builtin((*bash)->cmd[0]);
	if ((*bash)->built == -1)
	{
		if (execve(ft_access((*bash)->cmd[0],
					ft_path(envp)), (*bash)->cmd, envp) == -1)
		{
			fd_printf(2, "bash: %s: command not found\n", (*bash)->cmd[0]);
			exit(127);
		}
	}
	else
		ft_exec_builtin(bash, envp, def);
}

/* -> Esegue un comando singolo <- */
int	ft_lonely_cmd(t_bash **bash, char **envp, char *line)
{
	int	status;

	status = 0;
	(*bash)->built = ft_check_builtin((*bash)->cmd[0]);
	if ((*bash)->built == -1)
	{
		(*bash)->proc = fork();
		if ((*bash)->proc < 0)
			exit(errno);
		else if ((*bash)->proc == 0)
			ft_execve(bash, envp, line, 1);
		waitpid((*bash)->proc, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		return (1);
	}
	ft_execve(bash, envp, line, 0);
	return (1);
}

void	ft_check_new_cmd(t_bash **bash, char **cpy, char **envp)
{
	char	*line;
	t_bash	*tmp;

	tmp = *bash;
	while (tmp->next != NULL)
		tmp = tmp->next;
	if (((tmp->pipe[0] != 0 && tmp->pipe[1] != 0)
			|| (tmp->sep == '|' || tmp->sep == '&')) && tmp->next == NULL)
	{
		add_history(*cpy);
		line = readline("> ");
		if (!line)
		{
			fd_printf(2, "bash: syntax error: unexpected end of file\n");
			fd_printf(1, "\nExit\n");
			exit(2);
		}
		ft_parse(bash, line, envp);
	}
	return ;
}
