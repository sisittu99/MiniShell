#include "../INCL/minishell.h"

/* -> Analizza la stringa e cambia le Variabili col rispettivo valore.
	  Controlla inoltre che la Variabile non sia dentro agli '\'' <- */
char	*find_var_to_replace(char *line, char **envp, char re_dir)
{
	int	pos_dollar;
	int	pos_apex[2];
	int	i;

	i = 0;

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
		{
			line = ft_replace(line, envp, pos_dollar, &i);
			i++;
		}
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

/* -> Controlla gli errori di Sintassi sui separatori <- */
int	ft_syntax_err(char *line, int i)
{
	if (line[i] == '|' && line[i + 1] != '|')
		return (write(2, "bash: syntax error near unexpected token `|'\n", 57));
	else if (line[i] == '&' && line[i + 1] != '&')
		return (write(2, "bash: syntax error near unexpected token `&'\n", 57));
	else if (line[i] == '|' && line[i + 1] == '|')
		return (write(2, "bash: syntax error near unexpected token `||'\n", 57));
	else if (line[i] == '&' && line[i + 1] == '&')
		return (write(2, "bash: syntax error near unexpected token `&&'\n", 57));
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
		*i += 1;
		if (line[*i] == typequote)
			*i += 1;
		else if (ms_strchr(line, *i, typequote) > -1)
			while (line[*i] != typequote)
				*i += 1;
	}
	if (line[*i] == '|' || (line[*i] == '&' && line[*i + 1] == '&'))
	{
		if ((line[*i] == '|' && line[*i + 1] == '|')
			|| (line[*i] == '&' && line[*i + 1] == '&'))
		{
			if (ft_syntax_err(line, (*i + 2)) != 0)
				return (0);
			ft_init_node(bash, line, *j, (*i - *j));
			*j = *i + 1;
			*i += 1;
		}
		else
			ft_init_node(bash, line, *j, (*i - *j));
		*j = *i + 1;
	}
	else if (line[*i] == '<' || line[*i] == '>')
	{
		if ((line[*i] == '<' && line[*i + 1] == '<')
			|| (line[*i + 1] == '>' && line[*i + 1] == '>'))
		{
			if (ft_syntax_err(line, (*i + 2)) != 0)
				return (0);
			ft_init_node(bash, line, *j, (*i - *j));
			// *j = *i + 1;
		}
		else
			ft_init_node(bash, line, *j, (*i - *j));
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


	///////////////////////////////////////////////////////////
/*When running a list of commands separated by && (AND) or || (OR),	////////////////////////
the exit status of the command determines whether the next command in the list will be executed.*/
	////////////////////////////////////////////////////////////////////////////////////////


/* -> Fa il Parsing della stringa, dividendo i comandi grazie ai separatori
	  e creando un nodo per comando, pronto per essere passato all'exec <- */
void	ft_parse(t_bash **bash, char *line, char **envp)
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
		if (ft_check_sep(bash, line2, &i, &j) == 0)
			return ;
	}
	if (j < i)
		ft_init_node(bash, line2, j, (i - j));
	i = 1;
	tmp = *bash;
	while (tmp != NULL)
	{
		line3 = find_var_to_replace(ft_strdup(tmp->line), envp, tmp->re_dir);
//DA FARE FUNZIONE PER LA TILDE!!!!
//	line2 = ft_find_tilde(line2, envp);
		(tmp)->cmd = ms_split(line3);
		// ft_print_cmd((tmp)->cmd, i);
		// printf("Node: %d\t[%s]\tsep: %c   pipe: %d   re_dir: %c\n", i, line3, (tmp)->sep, (tmp)->pipe[0], (tmp)->re_dir);
		tmp = (tmp)->next;
		free(line3);
		i++;
	}
	free(line2);
}
