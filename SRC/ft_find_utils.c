#include "../INCL/minishell.h"

int	ft_check_apex(int *pos_apex, int pos_dollar, char *line)
{
	if (pos_apex[0] == -1 || pos_apex[1] == -1)	// $USER || '$USER
		return (1);
	while (pos_apex[1] < pos_dollar) //	''...$USER
	{
		pos_apex[0] = ms_strchr(line, pos_apex[1] + 1, '\'');
		pos_apex[1] = ms_strchr(line, pos_apex[0] + 1, '\'');
		if (pos_apex[0] == -1 || pos_apex[1] == -1)
			return (1);
	}
	if (pos_apex[0] < pos_dollar && pos_dollar < pos_apex[1]) //'$USER'
	{
		printf("here\n");
		if (pos_apex[2] < pos_apex[0] && pos_apex[1] < pos_apex[3]) // "'$USER'"
			return (printf("here2\n"));
		if (pos_apex[2] == -1 || pos_apex[3] == -1)
			return (0);
		while (pos_apex[3] < pos_apex[0]) //	""...'$USER'
		{
			pos_apex[2] = ms_strchr(line, pos_apex[3] + 1, '\"');
			pos_apex[3] = ms_strchr(line, pos_apex[2] + 1, '\"');
			if (pos_apex[2] == -1 || pos_apex[3] == -1)
				return (0);
		}
		if (pos_apex[2] < pos_apex[0] && pos_apex[1] < pos_apex[3])
			return (printf("here3\n"));
		else if (pos_apex[2] < pos_apex[0] && pos_apex[0] < pos_apex[3]
					&& pos_apex[2] < pos_dollar && pos_dollar < pos_apex[3])
			return (printf("here4\n"));
		return (0);
	}
	return (1);
}

char	*find_var_to_replace(char *line, char **envp, char re_dir)
{
	int	pos_dollar;
	int	pos_apex[4];
	int	i;

	i = 0;
	pos_dollar = ms_strchr(line, i, '$');
	pos_apex[0] = 0;
	pos_apex[1] = 1;
	while (pos_dollar != -1 && pos_apex[0] < pos_apex[1] && re_dir != '1')
	{
		pos_apex[0] = ms_strchr(line, i, '\'');
		pos_apex[1] = ms_strchr(line, (pos_apex[0] + 1), '\'');
		pos_apex[2] = ms_strchr(line, i, '\"');
		pos_apex[3] = ms_strchr(line, (pos_apex[2] + 1), '\"');
		printf("%d %d %d\n", pos_apex[2], pos_apex[3], pos_dollar); fflush(stdout);
		if (ft_check_apex(pos_apex, pos_dollar, line) != 0)
			ft_replace(&line, envp, pos_dollar, &i);
		else if (pos_apex[0] < pos_dollar && pos_dollar < pos_apex[1])
			i = pos_apex[1] + 1;
		else
			i = pos_dollar + 1;
		pos_dollar = ms_strchr(line, i, '$');
	}
	while (pos_dollar != -1 && re_dir != '1')
	{
		ft_replace(&line, envp, pos_dollar, &i);
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
