/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_two.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:30:53 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/23 11:13:12 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCL/minishell.h"
/*
	Gestisce il valore della variabile, a seconda che sia da riscriverne il
	valore o da scrivere in append.
*/
void	ft_env_var_found(char *cmd, t_bash **bash, int *index)
{
	char	*tmp;

	if (!ft_strchr(cmd, '='))
		return ;
	tmp = ft_strdup((*bash)->envp[index[0]]);
	free((*bash)->envp[index[0]]);
	if (cmd[index[2]] == '+' && (*bash)->envp[index[0]][index[1] + 1] == '=')
		(*bash)->envp[index[0]] = ft_strjoin(tmp, ft_substr(cmd, index[2] + 2,
					ft_strlen(cmd) - index[2] - 1));
	else
		(*bash)->envp[index[0]] = ft_strdup(cmd);
	free(tmp);
	return ;
}

/*
	banalmente, il ciclo che non entrava nella funzione ft_handle_env().
	Qualora trovi la variabile di cui a cmd, procede con ft_env_var_found().
*/
int	ft_handle_env_cycle(t_bash **bash, int *index, char *cmd, char *to_find)
{
	while ((*bash)->envp[index[0]])
	{
		while ((*bash)->envp[index[0]][index[1]] == to_find[index[2]])
		{
			if (!to_find[index[2] + 1])
			{
				ft_env_var_found(cmd, bash, index);
				return (1);
			}
			index[1]++;
			index[2]++;
		}
		index[0]++;
	}
	return (0);
}

/*
	gestisce la matrice di envp nel caso in cui export abbia argomenti.
*/
void	ft_handle_env(char *cmd, t_bash **bash)
{
	int		index[3];
	int		i;
	char	*to_find;
	char	**tmp;

	i = 0;
	while (i < 3)
		index[i++] = 0;
	to_find = cmd;
	if (ft_strchr(cmd, '=') != NULL)
	{
		i = 0;
		while (cmd[i++] != '=')
			;
		to_find = ft_substr(cmd, 0, i);
	}
	if (ft_handle_env_cycle(bash, index, cmd, to_find))
		return ;
	tmp = ft_new_env((*bash)->envp, 1);
	tmp[index[0]] = ft_strdup(cmd);
	tmp[index[0] + 1] = NULL;
	ft_free((*bash)->envp);
	(*bash)->envp = ft_new_env(tmp, 0);
	return ;
}
