/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_one.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:56:04 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/23 16:54:05 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCL/minishell.h"

/*
	inizializza la matrice sort per ft_sort_env().
*/
char	**ft_init_sort(char **envp, int *i)
{
	char	**sort;

	while (envp[i[0]])
		i[0]++;
	sort = (char **) malloc (sizeof(char *) * (i[0] + 1));
	if (!sort)
		return (NULL);
	i[0] = 0;
	while (envp[i[0]])
	{
		sort[i[0]] = ft_strdup(envp[i[0]]);
		i[0]++;
	}
	sort[i[0]] = NULL;
	return (sort);
}

/*
	Dai su, ci puoi arrivare!
	(solo con (char*))
*/
void	ft_swap(char **sort, int i)
{
	char	*tmp;

	tmp = ft_strdup(sort[i + 1]);
	free(sort[i + 1]);
	sort[i + 1] = ft_strdup(sort[i]);
	free(sort[i]);
	sort[i] = ft_strdup(tmp);
	free(tmp);
}

char	**ft_sort_env(char **envp)
{
	char	**sort;
	int		index[3];

	index[0] = 0;
	index[1] = 0;
	index[2] = 1;
	sort = ft_init_sort(envp, index);
	while (sort[index[1]])
	{
		while (index[2] < index[0] - index[1] - 1)
		{
			if (ft_strncmp(sort[index[2] + 1], sort[index[2]]) < 0)
				ft_swap(sort, index[2]);
			index[2]++;
		}
		index[2] = 0;
		index[1]++;
	}
	sort[index[0]] = NULL;
	return (sort);
}
