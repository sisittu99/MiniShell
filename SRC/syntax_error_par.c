/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_par.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcerchi <mcerchi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 19:12:35 by mcerchi           #+#    #+#             */
/*   Updated: 2022/06/30 19:12:38 by mcerchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

int	ft_par_error_b(int i, int j)
{
	if (i > j || i < j)
		fd_printf(2, "bash: syntax error near unexpected token `)'\n");
	else
		fd_printf(2, "bash: syntax error near unexpected token `('\n");
	return (0);
}

int	ft_par_error_help(char *line, int i, int j, int pos)
{
	if (ms_strchr(line, i, ')') != -1 && line[i + pos - 1] == ' ')
		return (fd_printf(2,
				"bash: syntax error near unexpected token `%s'\n",
				ft_substr(line, i + pos + 1, j - pos - 1)));
	else
		return (fd_printf(2,
				"bash: syntax error near unexpected token `('\n"));
}

/* -> Controlla gli errori di Sintassi sulle parentesi <- */
int	ft_par_error(char *line, int i, int j)
{
	int		pos;
	int		pos2;
	char	*tmp;

	tmp = ft_substr(line, i, j);
	pos = ms_strchr(tmp, 0, '(');
	if (pos != -1 && j < (int)ft_strlen(line))
		j += 1;
	pos2 = ms_strchr(tmp, 0, ')');
	free(tmp);
	if (pos != -1 && pos2 != -1)
	{
		if (pos < pos2)
			return (fd_printf(2,
					"bash: syntax error near unexpected token `%s'\n",
					ft_substr(line, pos + 1, pos2 - pos - 1)));
		else
			return (fd_printf(2,
					"bash: syntax error near unexpected token `)'\n"));
	}
	else if (pos2 != -1)
		return (fd_printf(2, "bash: syntax error near unexpected token `)'\n"));
	else if (pos != -1)
		return (ft_par_error_help(line, i, j, pos));
	return (0);
}
