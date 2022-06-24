/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:24:28 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/24 13:15:17 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCL/minishell.h"

char	**ft_delete_env_var( char **envp, int index[3])
{
	char	**cpy;
	int		i;

	i = 0;
	while (envp[i++])
		;
	cpy = (char **) malloc (sizeof(char *) * i);
	if (!cpy)
		return (envp);
	i = 0;
	while (envp[i])
	{
		if (i != index[0])
			cpy[i] = ft_strdup(envp[i]);
		i++;
	}
	return (cpy);
}

char	**ft_unset_find_var(char **envp, char *to_find)
{
	int	index[3];
	int	i;

	i = 0;
	while (i < 3)
		index[i++] = 0;
	while (envp[index[0]])
	{
		while (envp[index[0]][index[1]] == to_find[index[2]])
		{
			if (!to_find[index[2] + 1])
				return (ft_delete_env_var(envp, index));
			index[1]++;
			index[2]++;
		}
		index[0]++;
	}
	return (ft_new_env(envp, 0));
}

/*Elimina le variabili all'interno dell'env.
  Restituisce 0 se le elimina tutte, >0 se almeno una fallisce.
  Almeno, secondo il man di GNU... in realtà torna sempre 0.*/
int	ft_unset(t_bash **bash, char **cmd, char **envp)
{
	char	**new;
	char	*to_find;
	int		i;

	if (cmd[1] == NULL)
		return (0);
	if (ft_invalid_option(ft_new_env(cmd, 0)) == 1)
		return (1);
	(*bash)->envp = ft_new_env(envp, 0);
	i = 1;
	while (cmd[i])
	{
		to_find = ft_strdup(cmd[i]);
		new = ft_new_env((*bash)->envp, 0);
		ft_free((*bash)->envp);
		(*bash)->envp = ft_unset_find_var(new, to_find);
		ft_free(new);
		free(to_find);
		i++;
	}
	return (0);
}
