/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:56:09 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/21 15:56:09 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCL/minishell.h"

/*
	Nel caso in cui il caratterecontenga un uguale, procede a stampare
	tutta la riga reestante in mezzo alle virgolette.
	Questo è il comportamento tipico di export senza argomenti.
*/
int	ft_print_var_value(char *var, int j)
{
	if (var[j] == '=')
	{
		ft_putchar_fd(var[j++], 1);
		write(1, "\"", 1);
		while (var[j])
			ft_putchar_fd(var[j++], 1);
		write(1, "\"", 1);
	}
	return (j);
}

/*
	Funzione di stampa di export senza argomenti.
*/
int	ft_lonely_export(char **envp)
{
	char	**tmp;
	int		i;
	int		j;

	i = 1;
	j = 0;
	tmp = ft_new_env(envp, 0);
	tmp = ft_sort_env(envp);
	while (tmp[i])
	{
		ft_putstr_fd("declare -x ", 1);
		while (tmp[i][j])
		{
			j = ft_print_var_value(tmp[i], j);
			if (tmp[i][j] != '=')
				ft_putchar_fd(tmp[i][j++], 1);
		}
		ft_putchar_fd('\n', 1);
		j = 0;
		i++;
	}
	return (0);
}

/*
	Controlla che la variabile sia scritta in modo corretto;
	in più, fa partire la funzione di gestione della matrice envp.
*/
int	ft_export_cycle(t_bash **bash, char *cmd)
{
	int	j;

	j = 0;
	while (cmd[j])
	{
		if (!ft_isalpha(cmd[j]) && !ft_isdigit(cmd[j])
			&& cmd[j] != '_')
		{
			fd_printf(2, "bash: export: `%s\': not a valid identifier\n", cmd);
			return (1);
		}
		else if ((cmd[j + 1] == '\0' || cmd[j + 1] == '=')
			&& ((*bash)->pipe[0] == 0 && (*bash)->pipe[1] == 0)
			&& (*bash)->next == NULL)
		{
			ft_handle_env(cmd, bash);
			break ;
		}
		j++;
	}
	return (0);
}

int	ft_export(t_bash **bash, char **cmd, char **envp)
{
	int		i;
	int		j;
	int		status;

	i = 1;
	j = 0;
	if (cmd[1] == NULL)
		return (ft_lonely_export(envp));
	if (ft_invalid_option(cmd) == 1)
		return (1);
	if (((*bash)->pipe[0] == 0 && (*bash)->pipe[1] == 0)
		&& (*bash)->next == NULL)
		(*bash)->envp = ft_new_env(envp, 0);
	status = 0;
	while (cmd[i])
	{
		status = ft_export_cycle(bash, cmd[i]);
		i++;
	}
	return (status);
}
