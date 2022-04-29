#include "../INCL/minishell.h"

char *find_var_to_replace(char *line, char **envp)
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
		if (!(pos_apex[0] < pos_dollar && pos_dollar < pos_apex[1]))
		{
			line = ft_replace(line, envp, pos_dollar, &i);
			i++;
		}
		else if (pos_apex[0] < pos_dollar && pos_dollar < pos_apex[1])
			i = pos_apex[1];
		else
			i++;
		pos_dollar = ms_strchr(line, i, '$');
	}
	return (line);
}

void	ft_init_node(t_bash **bash, char *line, int pos, int len)
{
	t_bash	*tmp;
	char	sep[2];

	if (line[pos + len] == '|')
	{
		sep[0] = '|';
		sep[1] = '0';
	}
	else if ((line[pos + len] == '|' && line[pos + len + 1] == '|')				// DA SISTEMARE ! CONTROLLO NON SEMPRE CORRETTO
			|| (line[pos + len] == '&' && line[pos + len] == '&'))				// E NON GESTISCE ANCORA '>'
	{
		sep[0] = line[pos + len];
		sep[1] = line[pos + len];
	}
	else
	{
		sep[0] = '0';
		sep[1] = '0';
	}
	tmp = NULL;
	tmp = ft_new_node(line, pos, len, sep);
	ft_node_add_back(bash, tmp);
	tmp = NULL;
}

void	ft_parse(t_bash **bash, char *line, char **envp)
{
	int		i;
	int		j;
	char	typequote;

	i = 0;
	j = 0;
	line = find_var_to_replace(line, envp);				// Controllare per eventuali leaks //
	while (line[i] != '\0')
	{
		if ((line[i] == '\'' || line[i] == '\"'))
		{
			typequote = line[i++];
			if (ms_strchr(line, i, typequote) > -1)
				while (line[i + 1] != typequote)
					i++;
		}
		if (line[i] == '|' || (line[i] == '&' && line[i + 1] == '&'))
		{
			ft_init_node(bash, line, j, (i - j));
			j = i + 1;
		}
		// else if (line[i] == '<' || line[i] == '>')
		// {
		// 	ft_init_node(bash, line, j, (i - j));
		// 	j = i + 1;
		// }
		i++;
	}
	if (j < i)
		ft_init_node(bash, line, j, (i - j));
	i = 1;
	while (*bash != NULL)
	{
		printf("Node: %d\t[%s]\tsep: %c   pipe: %d\n", i, (*bash)->line, (*bash)->sep, (*bash)->pipe);
		*bash = (*bash)->next;
		i++;
	}
}
