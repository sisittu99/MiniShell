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

/* -> Quando presente, controlla che tipologia di redirect eseguire
	  e modifica la matrice, rimuovendolo <- */
int	ft_check_re_dir(t_bash **bash, int i, char *line)
{
	int		fd;
	char	*tmp;
	char	*buf;
	int		pip[2];
	struct sigaction	sa;

	sa.sa_handler = ft_sig_default;
	sa.sa_flags = SA_RESTART;

	if ((*bash)->proc == 0)
	{
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
	fd = 0;
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
		(*bash)->cmd = ft_delete_cmd((*bash)->cmd, i);
		return (1);
	}
	else if ((*bash)->cmd[i][0] == '<' && (*bash)->cmd[i][1] == '\0')
	{
		fd = open((*bash)->cmd[i + 1], O_RDONLY);
		if (fd == -1)
			exit(errno);
		dup2(fd, STDIN_FILENO);
		close(fd);
		(*bash)->cmd = ft_delete_cmd((*bash)->cmd, i);
		return (1);
	}
	else if ((*bash)->cmd[i][0] == '<' && (*bash)->cmd[i][1] == '<')
	{
		if (pipe(pip) == -1)
		{
			write(2, "error: could not create pipe\n", 32);
			exit(errno);
		}
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
		(*bash)->cmd = ft_delete_cmd((*bash)->cmd, i);
		dup2(pip[0], STDIN_FILENO);
		close(pip[0]);
		close(pip[1]);
		return (1);
	}
	return (0);
}

/* -> Controlla i redirect presenti ed esegue il comando <- */
void	ft_execve(t_bash **bash, char **envp, char *line, int def)
{
	int	i;

	i = 0;
	(void)line;
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
		if (execve(ft_access((*bash)->cmd[0], ft_path(envp)), (*bash)->cmd, envp) == -1)
		{
			fd_printf(2, "bash: %s: command not found\n", (*bash)->cmd[0]);
			exit(127);
		}
	}
	else
		ft_exec_builtin(bash, envp, def);
}

/* -> Esegue un comando singolo <- */
void	ft_lonely_cmd(t_bash **bash, char **envp, char *line)
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
			exit_status = WEXITSTATUS(status);
		return ;
	}
	ft_execve(bash, envp, line, 0);
	return ;
}

/* -> Funzione Helper che chiude tutti gli fd creati dalle pipe <- */
void	ft_close_pipe(t_bash **bash)
{
	t_bash	*tmp;

	tmp = *bash;
	close(tmp->pipe[0]);
	close(tmp->pipe[1]);
	while ((tmp->next->pipe[0] != 0 && tmp->next->pipe[1] != 0)
		&& tmp->next != NULL)
	{
		tmp = tmp->next;
		close(tmp->pipe[0]);
		close(tmp->pipe[1]);
	}
}

/* -> Quando chiamata esegue i comandi che sono separati dalla Pipe,
	   anche consecutivamente <- */
void	ft_pipe(t_bash **bash, char **envp, char *line)
{
	t_bash	*tmp;
	t_bash	*start;
	int		status;

	start = *bash;
	tmp = *bash;
	status = 0;
	(*bash)->proc = fork();
	if ((*bash)->proc < 0)
		exit(errno);
	else if ((*bash)->proc == 0)
	{
		dup2((*bash)->pipe[1], STDOUT_FILENO);
		ft_close_pipe(&start);
		ft_execve(bash, envp, line, 1);
	}
	while ((tmp->next->pipe[0] != 0 && tmp->next->pipe[1] != 0)
		&& tmp->next != NULL)
	{
		(*bash) = (*bash)->next;
		(*bash)->proc = fork();
		if ((*bash)->proc < 0)
			exit(errno);
		else if ((*bash)->proc == 0)
		{
			dup2(tmp->pipe[0], STDIN_FILENO);
			dup2((*bash)->pipe[1], STDOUT_FILENO);
			ft_close_pipe(&start);
			ft_execve(bash, envp, line, 1);
		}
		tmp = tmp->next;
	}
	*bash = (*bash)->next;
	(*bash)->proc = fork();
	if ((*bash)->proc < 0)
		exit(errno);
	else if ((*bash)->proc == 0)
	{
		dup2(tmp->pipe[0], STDIN_FILENO);
		ft_close_pipe(&start);
		ft_execve(bash, envp, line, 1);
	}
	ft_close_pipe(&start);
	tmp = start;
	while (start)
	{
		waitpid(start->proc, &status, 0);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		start = start->next;
	}
}

/* -> Gestisce l'esecuzione dei comandi, facendo controlli sia sui separatori,
	  che sui redirect, che sulle funzioni Builtin, ecc... <- */
void	ft_execute(t_bash **bash, char **envp, char **line)
{
	t_bash	*tmp;

	tmp = *bash;
	if ((*bash)->next == NULL)
	{
		ft_lonely_cmd(bash, envp, ft_strjoin(*line, "\n"));
		return ;
	}
	while (tmp)
	{
		if ((tmp->pipe[0] != 0 && tmp->pipe[1] != 0)
			&& tmp->next != NULL)
			ft_pipe(&tmp, envp, ft_strjoin(*line, "\n"));
		tmp = (tmp)->next;
	}
	return ;
}
