/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:47:34 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/22 16:47:34 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Inizializza il nuovo nodo assieme alle variabili standard,
	  inserendo la linea di comando, la pipe (se presente),
	 i separatori (se presenti), ecc. <- */
void	ft_init_node(t_bash **bash, char *line, int pos, int len)
{
	t_bash		*tmp;
	static char	sep[3];
	int			i;

	sep[0] = '0';
	tmp = NULL;
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
	tmp = ft_new_node(line, pos, len, sep);
	ft_node_add_back(bash, tmp);
	tmp = NULL;
	i = 0;
	while (i < 3)
		sep[i++] = 0;
}

/* -> Funzione helper per il controllo dei separatori che
	  sposta avnti il punbtatore in caso di parentesi <- */
void	ft_check_sep_help(char *line, int *i)
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
}

/* -> Controlla la presenza di quotes, separatori, pipes e re_directors,
	  chiamando poi la funzione 'ft_init_node' <- */
int	ft_check_sep(t_bash **bash, char *line, int *i, int *j)
{
	ft_check_sep_help(line, i);
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
			if (ft_syntax_err(line, ((*i) + 2)) != 0)
				return (0);
		ft_init_node(bash, line, (*j), ((*i) - (*j)));
	}
	return (1);
}

/* -> Funzione helper per il parser <- */
int	ft_parse_help(t_bash **bash, char **envp)
{
	t_bash	*tmp;
	char	*line3;

	line3 = NULL;
	if (ft_find_par(bash) == 0)
		return (0);
	tmp = *bash;
	while (tmp != NULL)
	{
		ft_find_tilde(&tmp->line, envp, tmp->re_dir, '~');
		ft_find_tilde(&tmp->line, envp, tmp->re_dir, '*');
		line3 = find_var_to_replace(tmp->line, envp, tmp->re_dir);
		(tmp)->cmd = ms_split(line3);
		if (line3)
			free(line3);
		tmp = (tmp)->next;
	}
	return (1);
}

/* -> Fa il Parsing della stringa, dividendo i comandi grazie ai separatori
	  e creando un nodo per comando, pronto per essere passato all'exec <- */
int	ft_parse(t_bash **bash, char *line, char **envp)
{
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (line[++i] != '\0')
	{
		if (ft_syntax_err_b(line, &j, i) != 0)
		{
			free(line);
			g_exit_status = 258;
			return (0);
		}
		if (ft_check_sep(bash, line, &i, &j) == 0)
		{
			free(line);
			g_exit_status = 258;
			return (0);
		}
	}
	if (j < i)
		ft_init_node(bash, line, j, (i - j));
	free(line);
	ft_parse_help(bash, envp);
	return (1);
}
