#include "../INCL/minishell.h"
/*
	esegue un comando in una sola riga.
*/

/* -> Funzione Helper che elimina due stringhe all'interno di una matrice <- */
char	**ft_delete_cmd(char **cmd, int pos)
{
	// printf("string in pos: %s\n", cmd[pos]);fflush(stdout);
	char **tmp;
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	len = 0;
	while (cmd[len])
		len++;
	tmp = (char **) malloc (sizeof(char *) * (len - 1));
	while(cmd[j] != NULL)
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

	if ((*bash)->cmd[i][0] == '>')
	{
		// printf("file name: %s\n", (*bash)->cmd[i + 1]); fflush(stdout);
		if ((*bash)->cmd[i][1] == '\0')
			fd = open((*bash)->cmd[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		else if ((*bash)->cmd[i][1] == '>' && (*bash)->cmd[i][2] == '\0')
			fd = open((*bash)->cmd[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (fd == -1)
		{
			strerror(errno);
			exit(errno);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		(*bash)->cmd = ft_delete_cmd((*bash)->cmd, i);
		// for (int index = 0; (*bash)->cmd[index]; index++)
		// 	{printf("cmd after delete: [%d] %s\n", index, (*bash)->cmd[index]); fflush(stdout);}
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
		// for (int index = 0; (*bash)->cmd[index]; index++)
		// 	{printf("cmd after delete: [%d] %s\n", index, (*bash)->cmd[index]); fflush(stdout);}
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
			if (ft_strcmp(buf, (*bash)->cmd[i + 1]) == 0)
				break ;
			tmp = ft_strjoin(buf, "\n");
			free(buf);
			buf = ft_strdup(line);
			free (line);
			line = ft_strjoin(buf, tmp);
			rl_replace_line(line, 0);			////////////////////////////////////////
			rl_redisplay();					//SISTEMARE!!!!!!!!!!            !!!!!!!!!!//
			// free(buf);
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
void	ft_execve(t_bash **bash, char **envp, char *line)
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
	// i = 0;
	// while ((*bash)->cmd[i])
		// printf("cmd: %s\n", (*bash)->cmd[0]);
	if (execve(ft_access((*bash)->cmd[0], ft_path(envp)), (*bash)->cmd, envp) == -1)
			write(2, "does not work man\n", 19);
		exit(errno);
}

/* -> Esegue un comando singolo <- */
void	ft_lonely_cmd(t_bash **bash, char **envp, char *line)
{
	(*bash)->proc = fork();
	if ((*bash)->proc < 0)
		exit(errno);
	else if ((*bash)->proc == 0)
		ft_execve(bash, envp, line);
	waitpid((*bash)->proc, NULL, 0);
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

	start = *bash;
	tmp = *bash;
	(*bash)->proc = fork();
	if ((*bash)->proc < 0)
		exit(errno);
	else if ((*bash)->proc == 0)
	{
		dup2((*bash)->pipe[1], STDOUT_FILENO);
		ft_close_pipe(&start);
		ft_execve(bash, envp, line);
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
			ft_execve(bash, envp, line);
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
		ft_execve(bash, envp, line);
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

/* -> Gestisce l'esecuzione dei comandi, facendo controlli sia sui separatori,
	  che sui redirect, che sulle funzioni Builtin, ecc... <- */
void	ft_execute(t_bash **bash, char **envp, char **line)
{
	t_bash	*tmp;
	// char	*dst;

	tmp = *bash;
	//caso singolo
	if ((*bash)->next == NULL)
	{
		ft_lonely_cmd(bash, envp, ft_strjoin(*line, "\n"));
		return ;
	}
	//altri casi
	while (tmp)
	{
		if ((tmp->pipe[0] != 0 && tmp->pipe[1] != 0)
		&& tmp->next != NULL)
			ft_pipe(&tmp, envp, ft_strjoin(*line, "\n"));
		// waitpid((tmp)->proc, NULL, 0);
		tmp = (tmp)->next;
	}
	// ft_free(line);
	return ;
}
