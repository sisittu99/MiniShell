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
void	ft_print_var_value(char **tmp, int *j, int i)
{
	ft_putchar_fd(tmp[i][(*j)++], 1);
	write(1, "\"", 1);
	while (tmp[i][(*j)])
		ft_putchar_fd(tmp[i][(*j)++], 1);
	write(1, "\"", 1);
}

int	ft_lonely_export(char **envp)
{
	char	**tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tmp = ft_sort_env(envp);
	while (tmp[i])
	{
		ft_putstr_fd("declare -x ", 1);
		while (tmp[i][j])
		{
			if (tmp[i][j] == '=')
				ft_print_var_value(tmp, &j, i);
			else
				ft_putchar_fd(tmp[i][j++], 1);
		}
		ft_putchar_fd('\n', 1);
		j = 0;
		i++;
	}
	ft_free(tmp);
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
		if ((cmd[j + 1] == '\0' || cmd[j] == '='
				|| (cmd[j + 1] == '=' && cmd[j] == '+'))
			&& ((*bash)->pipe[0] == 0 && (*bash)->pipe[1] == 0)
			&& (*bash)->next == NULL)
		{
			ft_handle_env(cmd, bash);
			break ;
		}
		if (!ft_isalpha(cmd[j]) && !ft_isdigit(cmd[j])
			&& cmd[j] != '_')
		{
			fd_printf(2, "bash: export: `%s\': not a valid identifier\n", cmd);
			return (1);
		}
		j++;
	}
	return (0);
}

int	ft_export(t_bash **bash, char **cmd, char **envp)
{
	int		i;
	int		status;

	i = 1;
	if (cmd[1] == NULL)
		return (ft_lonely_export(envp));
	if (ft_invalid_option(ft_new_env(cmd, 0)) == 1)
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
