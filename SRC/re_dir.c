/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   re_dir.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:24:31 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/22 15:04:34 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Funzione Helper che elimina due stringhe all'interno di una matrice <- */
void	ft_delete_cmd(t_bash **bash, int pos)
{
	char	**tmp;
	int		i[2];
	int		len;

	i[0] = 0;
	i[1] = 0;
	len = 0;
	while ((*bash)->cmd[len])
		len++;
	tmp = (char **) malloc (sizeof(char *) * len - 1);
	while ((*bash)->cmd[i[1]] != NULL)
	{
		if (pos == i[1])
			i[1] += 2;
		if ((*bash)->cmd[i[1]] != NULL)
		{
			tmp[i[0]] = ft_strdup((*bash)->cmd[i[1]]);
			i[0]++;
			i[1]++;
		}
	}
	tmp[len - 2] = 0;
	ft_free((*bash)->cmd);
	(*bash)->cmd = ft_new_env(tmp, 0);
	ft_free(tmp);
}

/* -> Funzione helper che crea il loop per il redirect dell'input <- */
void	ft_re_dir_loop(t_bash **bash, int i, int *pip, char *line)
{
	char	*tmp;
	char	*buf;

	while (1)
	{
		buf = readline("> ");
		if (ft_strcmp(buf, (*bash)->cmd[i + 1]) != 0)
			break ;
		tmp = ft_strjoin(buf, "\n");
		free(buf);
		buf = ft_strdup(line);
		free (line);
		line = ft_strjoin(buf, tmp);
		if (line && *line)
			add_history(line);
		ft_putstr_fd(tmp, pip[1]);
	}
}

/* -> Funzione helper che controlla il redirect dell'input <- */
int	ft_re_dir_help_b(t_bash **bash, int i, char *line)
{
	int		pip[2];

	if ((*bash)->cmd[i][0] == '<' && (*bash)->cmd[i][1] == '<')
	{
		if (pipe(pip) == -1)
		{
			fd_printf(2, "bash: error: could not create pipe\n");
			exit(errno);
		}
		ft_re_dir_loop(bash, i, pip, line);
		ft_delete_cmd(bash, i);
		dup2(pip[0], STDIN_FILENO);
		close(pip[0]);
		close(pip[1]);
		return (1);
	}
	return (0);
}

/* -> Funzione helper che controlla i redirect <- */
int	ft_re_dir_help(t_bash **bash, int i, int fd)
{
	if ((*bash)->cmd[i][0] == '>')
	{
		if ((*bash)->cmd[i][1] == '\0')
			fd = open((*bash)->cmd[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		else if ((*bash)->cmd[i][1] == '>' && (*bash)->cmd[i][2] == '\0')
			fd = open((*bash)->cmd[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (fd == -1)
			exit(errno);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		ft_delete_cmd(bash, i);
		return (1);
	}
	else if ((*bash)->cmd[i][0] == '<' && (*bash)->cmd[i][1] == '\0')
	{
		fd = open((*bash)->cmd[i + 1], O_RDONLY);
		if (fd == -1)
			exit(errno);
		dup2(fd, STDIN_FILENO);
		close(fd);
		ft_delete_cmd(bash, i);
		return (1);
	}
	return (0);
}

/* -> Quando presente, controlla che tipologia di redirect eseguire
	  e modifica la matrice, rimuovendo il simbolo <- */
int	ft_check_re_dir(t_bash **bash, int i, char *line)
{
	struct sigaction	sa;
	int					fd;

	fd = 0;
	sa.sa_handler = ft_sig_default;
	sa.sa_flags = SA_RESTART;
	if ((*bash)->proc == 0)
	{
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
	if (ft_re_dir_help(bash, i, fd) == 1)
		return (1);
	if (ft_re_dir_help_b(bash, i, line) == 1)
		return (1);
	return (0);
}
