/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_two.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:30:53 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/28 18:05:30 by fdrudi           ###   ########.fr       */
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
	if (cmd[index[2] + 1] == '+' && tmp[index[1] + 1] == '=')
	{
		(*bash)->envp[index[0]] = ft_strjoin(tmp, ft_substr(cmd, index[2] + 3,
					ft_strlen(cmd) - index[2] - 1));
	}
	else if (cmd[index[2] + 1] != '+')
		(*bash)->envp[index[0]] = ft_strdup(cmd);
	free(tmp);
	free(cmd);
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
			if ((!to_find[index[2] + 1] || to_find[index[2] + 1] == '+')
				&& (*bash)->envp[index[0]][index[1] + 1] == '=')
			{
				ft_env_var_found(ft_strdup(cmd), bash, index);
				free(to_find);
				return (1);
			}
			index[1]++;
			index[2]++;
		}
		index[0]++;
	}
	free(to_find);
	return (0);
}

void	ft_var_not_found(t_bash **bash, int *index, char *cmd)
{
	int		i;
	int		j;
	char	**tmp;

	i = 0;
	j = 0;
	tmp = ft_new_env((*bash)->envp, 1);
	if (ms_strchr(cmd, 0, '+') == -1)
		tmp[index[0]] = ft_strdup(cmd);
	else
	{
		tmp[index[0]] = (char *) malloc (sizeof(char) * (ft_strlen(cmd) + 1));
		while (cmd[i] != '\0')
		{
			if (cmd[i] != '+')
				tmp[index[0]][j++] = cmd[i];
			i++;
		}
		tmp[index[0]][j] = '\0';
	}
	tmp[index[0] + 1] = NULL;
	ft_free((*bash)->envp);
	(*bash)->envp = ft_new_env(tmp, 0);
	ft_free(tmp);
}

/*
	gestisce la matrice di envp nel caso in cui export abbia argomenti.
*/
void	ft_handle_env(char *cmd, t_bash **bash)
{
	int		index[3];
	int		i;
	char	*to_find;

	i = 0;
	while (i < 3)
		index[i++] = 0;
	to_find = ft_strdup(cmd);
	if (ft_strchr(cmd, '=') != NULL)
	{
		i = 0;
		while (cmd[i] != '=')
			i++;
		free(to_find);
		to_find = ft_substr(cmd, 0, i);
	}
	if (ft_handle_env_cycle(bash, index, cmd, to_find))
		return ;
	ft_var_not_found(bash, index, cmd);
	return ;
}
