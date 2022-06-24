char	*find_var_to_replace(char *line, char **envp, char re_dir)
{
	int	pos_dollar;
	int	pos_apex[4];
	int	i;

	i = 0;
	pos_dollar = ms_strchr(line, i, '$');
	pos_apex[0] = 0;
	pos_apex[1] = 1;
	while (pos_dollar != -1)
	{
		while (pos_apex[0] < pos_apex[1])
		{
			pos_apex[0] = ms_strchr(line, i, '\'');
			pos_apex[1] = ms_strchr(line, (pos_apex[0] + 1), '\'');
			pos_apex[2] = ms_strchr(line, i, '\"');
			pos_apex[3] = ms_strchr(line, (pos_apex[2] + 1), '\"');
			if (pos_apex[0] < pos_dollar && pos_dollar < pos_apex[1])
				pos_dollar = ms_strchr(line, i, '$');
			else if (pos_apex[0] == -1 || pos_apex[1] == -1 || pos_apex[0] > pos_dollar)
				ft_replace(&line, envp, pos_dollar, &i);
		}
		// // printf("%d %d %d\n", pos_apex[0], pos_apex[1], pos_dollar); fflush(stdout);
		// if (!(pos_apex[0] < pos_dollar && pos_dollar < pos_apex[1])
		// 	&& re_dir != '1')
		// 	ft_replace(&line, envp, pos_dollar, &i);
		else if (pos_apex[0] < pos_dollar && pos_dollar < pos_apex[1])
			i = pos_apex[1] + 1;
		else
			i++;
		pos_dollar = ms_strchr(line, i, '$');
	}
	return (line);
}

/*
	L'idea è quella di non trovare la prima coppia di virgolette e basta
	(provare il caso "echo 'ciao' '$USER'"), ma ciclare finché
		1) pos_apex[0] < pos_dollar < pos_apex[1]
			a) in tal caso, ciclare le virgolette per vedere
			in quale caso ci troviamo
		2) almeno un pos_apex non sia -1.
	Per rispettare la norma ci vogliono minimo 3 funzioni.
*/
