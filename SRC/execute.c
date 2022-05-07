#include "../INCL/minishell.h"
/*
	esegue un comando in una sola riga.
*/

char	**ft_delete_cmd(char **cmd, int pos)
{
	char **tmp;
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd[i])
		i++;
	tmp = (char **) malloc (sizeof(char *) * i - 1);
	i = 0;
	while(cmd[pos])
	{
		if (pos == i)
			j += 2;
		tmp[i++] = ft_strdup(cmd[j++]);
	}
	tmp[i] = 0;
	ft_free(cmd);
	return (tmp);
}

void	ft_check_re_dir(t_bash **bash, int i)
{
	int	fd;

	if ((*bash)->cmd[i] == ">")
	{
		fd = open((*bash)->cmd[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fd == -1)
		{
			strerror(errno);
			exit(errno);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		(*bash)->cmd = ft_delete_cmd((*bash)->cmd, i);
	}
	else if ((*bash)->cmd[i] == "<")
	{
		fd = open((*bash)->cmd[i + 1], O_RDONLY);
		if (fd == -1)
			exit(errno);
		dup2(fd, STDIN_FILENO);
		close(fd);
		(*bash)->cmd = ft_delete_cmd((*bash)->cmd, i);
	}
	// else if ((*bash)->cmd[i] == ">>")
	// {
		// (*bash)->cmd = ft_delete_cmd((*bash)->cmd, i);
	// }
	// else if ((*bash)->cmd[i] == "<<")
	// {
		// (*bash)->cmd = ft_delete_cmd((*bash)->cmd, i);
	// }
}

void	ft_execve(t_bash **bash, char **envp)
{
	int	i;

	i = 0;
	if ((*bash)->re_dir)
	{
		while ((*bash)->cmd[i])
		{
			ft_check_re_dir(bash, i);
			i++;
		}
	}
	if (execve(ft_access((*bash)->cmd[0], ft_path(envp)), (*bash)->cmd, envp) == -1)
			write(2, "does not work man\n", 19);
		exit(errno);
}

void	ft_lonely_cmd(t_bash **bash, char **envp)
{
	(*bash)->proc = fork();
	if ((*bash)->proc < 0)
		exit(errno);
	else if ((*bash)->proc == 0)
	{

	}
	waitpid((*bash)->proc, NULL, 0);
	return ;
}

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

void	ft_pipe(t_bash **bash, char **envp)
{
	t_bash	*tmp;
	t_bash	*start;

	start = *bash;
	tmp = *bash;
	(*bash)->proc = fork();
	if ((*bash)->proc < 0)
		exit(errno);
	else if ((*bash)->proc == 0)
	{
		dup2((*bash)->pipe[1], STDOUT_FILENO);
		ft_close_pipe(&start);
		if (execve(ft_access((*bash)->cmd[0], ft_path(envp)), (*bash)->cmd, envp) == -1)
			exit(errno);
	}
	while ((tmp->next->pipe[0] != 0 && tmp->next->pipe[1] != 0) && tmp->next != NULL)
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
			if (execve(ft_access((*bash)->cmd[0], ft_path(envp)), (*bash)->cmd, envp) == -1)
				exit(errno);
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
		if (execve(ft_access((*bash)->cmd[0], ft_path(envp)), (*bash)->cmd, envp) == -1)
			exit(errno);
	}
	ft_close_pipe(&start);
	while(wait(NULL) > 0);
}

/*
	SECONDA FASE DEL PARSING
	Qui praticamente dovranno partire tutti i controlli dei separatori e di conseguenza delle varie casistiche.
	Casistiche:
	///	1)	Caso singolo:	Apriamo il processo, eseguiamo e chiudiamo.
	///	2)	Caso '|':		Pipe(int[2]); apriamo il processo; gestiamo la pipe[0] del precedente e la pipe[1] dell'attuale processo; chiudiamo tutti gli altri fd; eseguiamo.
		3)	Caso '||':		Apriamo il processo, eseguiamo, se fallisce passiamo al nodo successivo altrimenti chiudiamo.
		4)	Caso '&&':		Apriamo il processo, eseguiamo, passiamo al nodo successivo finché abbiamo nodi, chiudiamo.
		5)	Caso '<':		Apriamo il processo, gestiamo l'input che sarà il file (bash->next), eseguiamo e chiudiamo.
		6)	Caso '>':		Apriamo il processo, gestiamo l'output che sarà il file (bash->next), eseguiamo e chiudiamo.
		7)	Caso '<<':		APPEND: Apriamo un readline costante che raccoglie il nuovo input finché non verrà scritta la parola chiave (bash->next). Inseriamo in un file tmp, al ché gestiamo come al caso (5).
		8)	Caso '>>':		Come il caso (6), ma non si sovrascrive il file bensì lo si incolla a fine file.

	NB:	Controllare il caso dei file o delle directory!
*/

void	ft_execute(t_bash **bash, char **envp)
{
	t_bash	*tmp;

	tmp = *bash;
	//caso singolo
	if ((*bash)->next == NULL)
	{
		ft_lonely_cmd(bash, envp);
		return ;
	}
	//altri casi
	while (tmp)
	{
		if ((tmp->pipe[0] != 0 && tmp->pipe[1] != 0)
		&& tmp->next != NULL)
			ft_pipe(&tmp, envp);
		// (tmp)->proc = fork();
		// if ((tmp)->proc < 0)
		// 	exit(errno);
		// else if ((tmp)->proc == 0)
		// {
		// 	if (execve(ft_access((tmp)->cmd[0], ft_path(envp)), (tmp)->cmd, envp) == -1)
		// 		write(2, "does not work man\n", 19);
		// 	exit(errno);
		// }
		waitpid((tmp)->proc, NULL, 0);
		tmp = (tmp)->next;
	}
	return ;
}
