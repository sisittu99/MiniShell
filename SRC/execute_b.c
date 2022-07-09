/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_b.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcerchi <mcerchi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:24:40 by fdrudi            #+#    #+#             */
/*   Updated: 2022/07/09 16:51:17 by mcerchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Controlla i redirect presenti ed esegue il comando <- */
void	ft_execve(t_bash **bash, char **envp, char *line, int def)
{
	int	i;

	i = 0;
	if ((*bash)->built == -2)
		(*bash)->built = ft_check_builtin((*bash)->cmd[0]);
	if ((*bash)->re_dir)
	{
		while ((*bash)->cmd[i])
		{
			if (ft_check_re_dir(bash, i, line) == 0)
				i++;
		}
	}
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
	if ((*bash)->built == -1 || (*bash)->re_dir)
	{
		(*bash)->proc = fork();
		if ((*bash)->proc < 0)
			exit(errno);
		else if ((*bash)->proc == 0)
			ft_execve(bash, envp, line, 1);
		waitpid((*bash)->proc, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		free(line);
		return (1);
	}
	ft_execve(bash, envp, line, 0);
	free(line);
	return (1);
}

char	*ft_check_new_cmd_help(char *cpy)
{
	char	*line;

	add_history(cpy);
	line = readline("> ");
	if (!line)
	{
		fd_printf(2, "bash: syntax error: unexpected end of file\n");
		fd_printf(1, "\nExit\n");
		exit(2);
	}
	return (line);
}

void	ft_check_new_cmd(char **cpy)
{
	char	*line;
	char	*tmp;
	char	*tmp2;
	int		i;

	line = NULL;
	tmp2 = NULL;
	tmp = ft_strtrim(*cpy, " ");
	i = ft_strlen(tmp) - 1;
	while (tmp[i] == '|'
		|| (tmp[i] == '&' && tmp[i - 1] == '&' && tmp[i - 2] != '&')
		|| (tmp[i] == '|' && tmp[i - 1] == '|' && tmp[i - 2] != '|'))
	{
		line = ft_check_new_cmd_help(*cpy);
		tmp2 = ft_strdup(*cpy);
		free(*cpy);
		*cpy = ft_replace_join(tmp2, ft_strdup(" "), line);
		free(tmp);
		tmp = ft_strtrim(*cpy, " ");
		i = ft_strlen(tmp) - 1;
	}
	free(tmp);
	return ;
}
