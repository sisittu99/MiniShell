/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 15:20:13 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/28 19:18:49 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

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
		{
			printf("HERE\n");
			return (fd_printf(2,
					"bash: syntax error near unexpected token `%s'\n",
					ft_substr(line, pos + 1, pos2 - pos - 1)));
		}
		else
			return (fd_printf(2,
					"bash: syntax error near unexpected token `)'\n"));
	}
	else if (pos2 != -1)
		return (fd_printf(2, "bash: syntax error near unexpected token `)'\n"));
	else if (pos != -1)
	{
		if (ms_strchr(line, i, ')') != -1 && line[i + pos - 1] == ' ')
			return (fd_printf(2,
					"bash: syntax error near unexpected token `%s'\n",
					ft_substr(line, i + pos + 1, j - pos - 1)));
		else
			return (fd_printf(2, "bash: syntax error near unexpected token `('\n"));
	}
	return (0);
}

/* -> Funzione helper per il controllo dei token <- */
int	ft_syntax_err_help(char *line, int *def, int i)
{
	while (line[++i] == ' ')
	{
		if (line[i + 1] == '|' || line[i + 1] == '<'
			|| line[i + 1] == '>')
			return (fd_printf(2,
					"bash: syntax error near unexpected token `%c'\n",
					line[i + 1]));
		if (line[i + 1] == '\0')
			*def = i + 1;
	}
	return (0);
}

/* -> Controlla gli errori di Sintassi per i token disabilitati <- */
int	ft_syntax_err_b(char *line, int *def, int i)
{
	if (line[i] == '$' && line[i + 1] == '(')
		return (fd_printf(2,
				"bash: syntax error: token `$()' has been disabled\n"));
	else if ((line[i] == '|' || line[i] == '<' || line[i] == '>')
		&& line[i + 1] == '&')
		return (fd_printf(2,
				"bash: syntax error: token `&' has been disabled\n"));
	else if ((line[i] == '&' && line[i + 1] != '&')
		|| (line[i] == '&' && line[i + 1] == '\0'))
		return (fd_printf(2,
				"bash: syntax error: token `&' has been disabled\n"));
	else if ((line[i] == '>' || line[i] == '<') && (line[i + 1] == '\0'
			|| line[i + 1] == '|' || line[i + 1] == '&'))
		return (fd_printf(2,
				"bash: syntax error near unexpected token `newline'\n"));
	else if ((line[i] == '|' || line[i] == '<' || line[i] == '>')
		&& line[i + 1] == ' ')
		return (ft_syntax_err_help(line, def, i));
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
