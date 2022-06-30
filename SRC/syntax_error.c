/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcerchi <mcerchi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 15:20:13 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/30 19:12:15 by mcerchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

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
