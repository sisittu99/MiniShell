#include "../INCL/minishell.h"

int	*ms_bzero(int size)
{
	int	i;
	int	*cp;

	if (size <= 0)
		return (0);
	cp = (int *) malloc (sizeof(int) * size);
	if (!cp)
		return (0);
	i = -1;
	while (++i < size)
		cp[i] = 0;
	return (cp);
}

void	ft_find_tilde(char **line, char **envp, char re_dir, char c)
{
	int	pos;
	int	pos_apex[4];
	int	i;

	i = 0;
	pos_apex[0] = 0;
	pos_apex[1] = 1;
	pos_apex[2] = 0;
	pos_apex[3] = 1;
	pos = ms_strchr(*line, i, c);
	while (pos != -1)
	{
		if (pos_apex[0] < pos_apex[1])
		{
			pos_apex[0] = ms_strchr(*line, i, '\'');
			pos_apex[1] = ms_strchr(*line, (pos_apex[0] + 1), '\'');
		}
		if (pos_apex[2] < pos_apex[3])
		{
			pos_apex[2] = ms_strchr(*line, i, '\"');
			pos_apex[3] = ms_strchr(*line, (pos_apex[2] + 1), '\"');
		}
		if (!(pos_apex[0] < pos && pos < pos_apex[1])
			&& !(pos_apex[2] < pos && pos < pos_apex[3])
			&& re_dir != '1')
		{
			ft_replace(line, envp, pos, &i);
			i++;
		}
		else if ((pos_apex[0] < pos && pos < pos_apex[1])
				&& (pos_apex[2] < pos && pos < pos_apex[3]))
			i = pos_apex[1] > pos_apex[3] ? pos_apex[1] + 1 : pos_apex[3] + 1;
		else if (pos_apex[0] < pos && pos < pos_apex[1])
			i = pos_apex[1] + 1;
		else if (pos_apex[2] < pos && pos < pos_apex[3])
			i = pos_apex[3] + 1;
		else
			i++;
		pos = ms_strchr(*line, i, c);
	}

}

/* -> Analizza la stringa e cambia le Variabili col rispettivo valore.
	  Controlla inoltre che la Variabile non sia dentro agli '\'' <- */
char	*find_var_to_replace(char *line, char **envp, char re_dir)
{
	int	pos_dollar;
	int	pos_apex[2];
	int	i;

	i = 0;
	ft_find_tilde(&line, envp, re_dir, '~');
	ft_find_tilde(&line, envp, re_dir, '*');
	pos_dollar = ms_strchr(line, i, '$');
	pos_apex[0] = 0;
	pos_apex[1] = 1;
	while (pos_dollar != -1)
	{
		if (pos_apex[0] < pos_apex[1])
		{
			pos_apex[0] = ms_strchr(line, i, '\'');
			pos_apex[1] = ms_strchr(line, (pos_apex[0] + 1), '\'');
		}
		if (!(pos_apex[0] < pos_dollar && pos_dollar < pos_apex[1]) && re_dir != '1')
			ft_replace(&line, envp, pos_dollar, &i);
		else if (pos_apex[0] < pos_dollar && pos_dollar < pos_apex[1])
			i = pos_apex[1] + 1;
		else
			i++;
		pos_dollar = ms_strchr(line, i, '$');
	}
	return (line);
}

/* -> Inizializza il nuovo nodo assieme alle variabili standard,
	  inserendo la linea di comando, la pipe (se presente),
	 i separatori (se presenti), ecc. <- */
void	ft_init_node(t_bash **bash, char *line, int pos, int len)
{
	t_bash		*tmp;
	static char	sep[3];
	int			i;

	sep[0] = '0';
	if (line[pos + len] == '|' && line[pos + len + 1] != '|')
		sep[0] = '1';
	else if ((line[pos + len] == '|' && line[pos + len + 1] == '|')
		|| (line[pos + len] == '&' && line[pos + len + 1] == '&'))
		sep[1] = line[pos + len];
	else if (line[pos + len] == '>' || line[pos + len] == '<')
	{
		sep[2] = '1';
		return ;
	}
	tmp = NULL;
	tmp = ft_new_node(line, pos, len, sep);
	ft_node_add_back(bash, tmp);
	tmp = NULL;
	i = 0;
	while (i < 3)
		sep[i++] = 0;
}

int	ft_syntax_err_b(char *line, int *def, int i)
{
	if (line[i] == '$' && line[i + 1] == '(')
		return (fd_printf(2, "bash: syntax error: token `$()' has been disabled\n"));
	else if ((line[i] == '|' || line[i] == '<' || line[i] == '>')
		 && line[i + 1] == '&')
		return (fd_printf(2, "bash: syntax error: token `&' has been disabled\n"));
	else if ((line[i] == '&' && line[i + 1] != '&') || (line[i] == '&' && line[i + 1] == '\0'))
		return (fd_printf(2, "bash: syntax error: token `&' has been disabled\n"));
	else if ((line[i] == '>' || line[i] == '<') && (line[i + 1] == '\0'
				|| line[i + 1] == '|' || line[i + 1] == '&'))
		return (fd_printf(2, "bash: syntax error near unexpected token `newline'\n"));
	else if ((line[i] == '|' || line[i] == '<' || line[i] == '>')
		 && line[i + 1] == ' ')
	{
		while (line[++i] == ' ')
		{
			if (line[i + 1] == '|' || line[i + 1] == '<'
				|| line[i + 1] == '>')
				return (fd_printf(2, "bash: syntax error near unexpected token `%c'\n", line[i + 1]));
			if (line[i + 1] == '\0')
				*def = i + 1;
		}
	}
	return (0);
}

/* -> Controlla gli errori di Sintassi sui separatori <- */
int	ft_syntax_err(char *line, int i)
{
	if (line[i] == '|' && line[i + 1] != '|')
		return (fd_printf(2, "bash: syntax error near unexpected token `|'\n"));
	else if (line[i] == '&' && line[i + 1] != '&')
		return (fd_printf(2, "bash: syntax error near unexpected token `&'\n"));
	else if (line[i] == '|' && line[i + 1] == '|')
		return (fd_printf(2, "bash: syntax error near unexpected token `||'\n"));
	else if (line[i] == '&' && line[i + 1] == '&')
		return (fd_printf(2, "bash: syntax error near unexpected token `&&'\n"));
	else if ((line[i] == '<' && line[i + 1] != '<'))
		return (fd_printf(2, "bash: syntax error near unexpected token `<'\n"));
	else if ((line[i] == '>' && line[i + 1] != '>'))
		return (fd_printf(2, "bash: syntax error near unexpected token `>'\n"));
	else if ((line[i] == '<' && line[i + 1] == '<'))
		return (fd_printf(2, "bash: syntax error near unexpected token `<<'\n"));
	else if ((line[i] == '>' && line[i + 1] == '>'))
		return (fd_printf(2, "bash: syntax error near unexpected token `>>'\n"));
	return (0);
}

/* -> Controlla la presenza di quotes, separatori, pipes e re_directors,
	  chiamando poi la funzione 'ft_init_node' <- */
int	ft_check_sep(t_bash **bash, char *line, int *i, int *j)
{
	char	typequote;

	if ((line[*i] == '\'' || line[*i] == '\"'))
	{
		typequote = line[*i];
		(*i) += 1;
		if (line[(*i)] == typequote)
			(*i) += 1;
		else if (ms_strchr(line, (*i), typequote) > -1)
			while (line[(*i)] != typequote)
				(*i) += 1;
	}
	if (line[(*i)] == '|' || (line[(*i)] == '&' && line[(*i) + 1] == '&'))
	{
		if ((line[(*i)] == '|' && line[(*i) + 1] == '|')
			|| (line[(*i)] == '&' && line[(*i) + 1] == '&'))
		{
			if (ft_syntax_err(line, ((*i) + 2)) != 0)
				return (0);
			ft_init_node(bash, line, (*j), ((*i) - (*j)));
			(*j) = (*i) + 1;
			(*i) += 1;
		}
		else
			ft_init_node(bash, line, (*j), ((*i) - (*j)));
		(*j) = (*i) + 1;
	}
	else if (line[(*i)] == '<' || line[(*i)] == '>')
	{
		if ((line[(*i)] == '<' && line[(*i) + 1] == '<')
			|| (line[(*i) + 1] == '>' && line[(*i) + 1] == '>'))
		{
			if (ft_syntax_err(line, ((*i) + 2)) != 0)
				return (0);
			ft_init_node(bash, line, (*j), ((*i) - (*j)));
			// (*j) = (*i) + 1;
		}
		else
			ft_init_node(bash, line, (*j), ((*i) - (*j)));
	}
	return (1);
}

void	ft_print_cmd(char **cmd, int nbr)
{
	int	i;

	i = 0;
	printf("Node: %d\n", nbr);
	while (cmd[i] != NULL)
	{
		printf("%s\n", cmd[i]);
		i++;
	}
}


int	ft_par_error(char *line, int i, int j)
{
	int	pos;
	int	pos2;

	pos = ms_strchr(ft_substr(line, i, j + 1), 0, '(');
	if (pos != -1 && j < (int)ft_strlen(line))
		j += 1;
	pos2 = ms_strchr(ft_substr(line, i, j + 1), 0, ')');
	if (pos != -1 && pos2 != -1)
	{
		if (pos < pos2)
			return (fd_printf(2, "bash: syntax error near unexpected token `%s'\n", ft_substr(line, pos + 1, pos2 - pos - 1)));
		else
			return (fd_printf(2, "bash: syntax error near unexpected token `)'\n"));
	}
	else if (pos2 != -1)
		return (fd_printf(2, "bash: syntax error near unexpected token `)'\n"));
	else if (pos != -1)
		return (fd_printf(2, "bash: syntax error near unexpected token `('\n"));
	return (0);
}

int	ft_nbr_par(char **line)
{
	int		i;
	int		j;
	int		pos[2];
	char	*tmp;

	i = 0;
	j = ft_strlen((*line)) - 1;
	while ((*line)[i] == '(')
		i++;
	while ((*line)[j] == ')')
		j--;
	if (ft_par_error(*line, i, j) != 0)
			return (300);
	pos[0] = i;
	pos[1] = ft_strlen((*line)) - 1 - j;
	tmp = ft_substr((*line), i, j + 1);
	free(*line);
	(*line) = ft_strdup(tmp);
	free(tmp);
	return (pos[0] - pos[1]);
}


int	ft_find_par(t_bash **bash)
{
	t_bash	*tmp;
	int		lvl;
	int		new_lvl;

	tmp = *bash;
	lvl = ft_nbr_par(&tmp->line);
	if (lvl < 0 || lvl == 300)
		return (0);
	tmp->par = lvl;
	while (tmp->next)
	{
		new_lvl = ft_nbr_par(&tmp->next->line);
		if (new_lvl == 300)
			return (0);
		if (new_lvl >= 0)
			tmp->next->par = tmp->par + new_lvl;
		else
			tmp->next->par = tmp->par;
		if (lvl < 0)
			tmp->next->par += lvl;
		tmp = tmp->next;
		lvl = new_lvl;
	}
	if (tmp->par + lvl != 0)
		return (0);
	return (1);
}

/* -> Fa il Parsing della stringa, dividendo i comandi grazie ai separatori
	  e creando un nodo per comando, pronto per essere passato all'exec <- */
int	ft_parse(t_bash **bash, char *line, char **envp)
{
	t_bash	*tmp;
	int		i;
	int		j;
	char	*line2;
	char	*line3;

	i = -1;
	j = 0;
	line2 = ft_strdup(line);
	while (line2[++i] != '\0')
	{
		if (ft_syntax_err_b(line2, &j, i) != 0)
		{
			exit_status = 258;
			return (0);
		}
		if (ft_check_sep(bash, line2, &i, &j) == 0)
		{
			exit_status = 258;
			return (0);
		}
	}
	if (j < i)
		ft_init_node(bash, line2, j, (i - j));
	i = 1;
	if (ft_find_par(bash) == 0)
		return (0);
	tmp = *bash;
	while (tmp != NULL)
	{
		line3 = find_var_to_replace(ft_strdup(tmp->line), envp, tmp->re_dir);
		(tmp)->cmd = ms_split(line3);
		// ft_print_cmd((tmp)->cmd, i);
		printf("Node: %d\t[%s]\tsep: %c\tpipe: %d\tre_dir: %c\tpar: %d\n", i, tmp->line, (tmp)->sep, (tmp)->pipe[0], (tmp)->re_dir, tmp->par);
		tmp = (tmp)->next;
		free(line3);
		i++;
	}
	free(line2);
	return (1);
}
