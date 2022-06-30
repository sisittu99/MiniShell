/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcerchi <mcerchi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 18:46:54 by mcerchi           #+#    #+#             */
/*   Updated: 2022/06/30 18:46:54 by mcerchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/*
	Caso in cui la variabile si trova certamente all'interno degli apici.
*/
int	ft_check_dollar_inside_apex(int *pos_apex, int pos_dollar, char *line)
{
	if (pos_apex[2] < pos_apex[0] && pos_apex[1] < pos_apex[3])
		return (1);
	if (pos_apex[2] == -1 || pos_apex[3] == -1)
		return (0);
	while (pos_apex[3] < pos_apex[0])
	{
		pos_apex[2] = ms_strchr(line, pos_apex[3] + 1, '\"');
		pos_apex[3] = ms_strchr(line, pos_apex[2] + 1, '\"');
		if (pos_apex[2] == -1 || pos_apex[3] == -1)
			return (0);
	}
	if (pos_apex[2] < pos_apex[0] && pos_apex[1] < pos_apex[3])
		return (1);
	else if (pos_apex[2] < pos_apex[0] && pos_apex[0] < pos_apex[3]
		&& pos_apex[2] < pos_dollar && pos_dollar < pos_apex[3])
		return (1);
	return (0);
}

/*
	Controlla che la variabile non si trovi all'interno di apici.
*/
int	ft_check_apex(int *pos_apex, int pos_dollar, char *line)
{
	if (pos_apex[0] == -1 || pos_apex[1] == -1)
		return (1);
	while (pos_apex[1] < pos_dollar)
	{
		pos_apex[0] = ms_strchr(line, pos_apex[1] + 1, '\'');
		pos_apex[1] = ms_strchr(line, pos_apex[0] + 1, '\'');
		if (pos_apex[0] == -1 || pos_apex[1] == -1)
			return (1);
	}
	if (pos_apex[0] < pos_dollar && pos_dollar < pos_apex[1])
		return (ft_check_dollar_inside_apex(pos_apex, pos_dollar, line));
	return (1);
}

/*
	Assegna il valore delle posizioni di apici e virgolette.
	Nello specifico, 0 e 1 sono per '',
					 2 e 3 sono per "".
*/
void	ft_find_apex(int *pos_apex, char **line, int i)
{
	pos_apex[0] = ms_strchr(*line, i, '\'');
	pos_apex[1] = ms_strchr(*line, (pos_apex[0] + 1), '\'');
	pos_apex[2] = ms_strchr(*line, i, '\"');
	pos_apex[3] = ms_strchr(*line, (pos_apex[2] + 1), '\"');
}

/*
	Cerca la variabile da cambiare, soprattutto cerca di capire
	se la variabile non si trovi dentro a coppie di apici debilitanti.
*/
char	*find_var_to_replace(char **line, char **envp, char re_dir)
{
	int	pos_dollar;
	int	pos_apex[4];
	int	i;

	i = 0;
	pos_dollar = ms_strchr(*line, i, '$');
	ft_find_apex(pos_apex, line, i);
	while (pos_dollar != -1 && pos_apex[0] < pos_apex[1] && re_dir != '1')
	{
		ft_find_apex(pos_apex, line, i);
		if (ft_check_apex(pos_apex, pos_dollar, *line) != 0)
			ft_replace(line, envp, pos_dollar, &i);
		else if (pos_apex[0] < pos_dollar && pos_dollar < pos_apex[1])
			i = pos_apex[1] + 1;
		else
			i = pos_dollar + 1;
		pos_dollar = ms_strchr(*line, i, '$');
	}
	while (pos_dollar != -1 && re_dir != '1')
	{
		ft_replace(line, envp, pos_dollar, &i);
		i++;
		pos_dollar = ms_strchr(*line, i, '$');
	}
	return (*line);
}

/*
	L'idea è quella di non trovare la prima coppia di virgolette e basta
	(provare il caso "echo 'ciao' '$USER'"), ma ciclare finché
		1) pos_apex[0] < pos_dollar < pos_apex[1]
			a) in tal caso, ciclare le virgolette per vedere
			in quale caso ci troviamo
		2) almeno un pos_apex non sia -1.
*/
