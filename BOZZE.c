#include "./INCL/minishell.h"

int	main()
{
	// char	*term;
	// char	buffer[2048];
	// int		get_term;

	// term = getenv("TERM");
	// printf("Term : %s\n", term);
	// get_term = tgetent(buffer, term);
	// return (0);
  const char *name;
  char  *bp;
  char  *term;
  int   height;
  int   width;

  bp = malloc(sizeof(*bp));
  name = "TERM";
  if ((term = getenv(name)) == NULL)
    return (1);
  printf("My terminal is %s.\n", term);
  tgetent(bp, term);
  height = tgetnum ("li");
  width = tgetnum ("co");
  printf("H : %d\nL : %d\n", height, width);
  free(bp);
  return (0);
}

////////////////////////////////////////////////////////////////

// void	ft_init_node(t_bash **bash, char *line, int pos, int len)
// {
// 	t_bash	*tmp;
// 	char	sep[4];
// 	int		i;

// 	i = 0;
// 	while (i < 4)
// 		sep[i++] = '0';
// 	if (line[pos + len] == '|' && line[pos + len + 1] != '|')
// 		sep[0] = '1';
// 	else if ((line[pos + len] == '|' && line[pos + len + 1] == '|')
// 			|| (line[pos + len] == '&' && line[pos + len + 1] == '&'))
// 		sep[1] = line[pos + len];
// 	else if (line[pos + len] == '>' || line[pos + len] == '<')
// 	{
// 		sep[2] = line[pos + len];
// 		if ((line[pos + len] == '>' && line[pos + len + 1] == '>')
// 			|| (line[pos + len] == '<' && line[pos + len + 1] == '<'))
// 			sep[3] = line[pos + len];
// 	}
// 	tmp = NULL;
// 	tmp = ft_new_node(line, pos, len, sep);
// 	ft_node_add_back(bash, tmp);
// 	tmp = NULL;
// }


// int	ft_check_sep(t_bash **bash, char *line, int *i, int *j)
// {
// 	char	typequote;

// 	if ((line[*i] == '\'' || line[*i] == '\"'))
// 	{
// 		typequote = line[*i += 1];
// 		if (ms_strchr(line, *i, typequote) > -1)
// 			while (line[*i + 1] != typequote)
// 				*i += 1;
// 	}
// 	if (line[*i] == '|' || (line[*i] == '&' && line[*i + 1] == '&'))
// 	{
// 		if ((line[*i] == '|' && line[*i + 1] == '|') ||
// 			(line[*i] == '&' && line[*i + 1] == '&'))
// 		{
// 			if (ft_syntax_err(line, (*i + 2)) != 0)
// 				return (0);
// 			ft_init_node(bash, line, *j, (*i - *j));
// 			*j = *i + 1;
// 			*i += 1;
// 		}
// 		else
// 			ft_init_node(bash, line, *j, (*i - *j));
// 		*j = *i + 1;
// 	}
// 	else if (line[*i] == '<' || line[*i] == '>')
// 	{
// 		if ((line[*i] == '<' && line[*i + 1] == '<') ||
// 			(line[*i + 1] == '>' && line[*i + 1] == '>'))
// 		{
// 			if (ft_syntax_err(line, (*i + 2)) != 0)
// 				return (0);
// 			ft_init_node(bash, line, *j, (*i - *j));
// 			*j = *i + 1;
// 			*i += 1;
// 		}
// 		else
// 			ft_init_node(bash, line, *j, (*i - *j));
// 		*j = *i + 1;
// 	}
// 	return (1);
// }
