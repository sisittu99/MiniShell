/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_one.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcerchi <mcerchi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 15:56:04 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/22 22:12:14 by mcerchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../INCL/minishell.h"

/*
	inizializza la matrice sort per ft_sort_env().
*/
char	**ft_init_sort(char **envp)
{
	char	**sort;
	int		i;

	i = 0;
	while (envp[i++])
		;
	sort = (char **) malloc(sizeof(char *) * (i + 1));
	if (!sort)
		return (NULL);
	i = -1;
	while (envp[++i])
		sort[i] = ft_strdup(envp[i]);
	sort[i] = NULL;
	return (sort);
}

/*
	Dai su, ci puoi arrivare!
	(solo con (char*))
*/
void	ft_swap(char **a, char **b)
{
	char	*tmp;

	tmp = ft_strdup(*a);
	*a = ft_strdup(*b);
	*b = ft_strdup(tmp);
}

char	**ft_sort_env(char **envp)
{
	char	**sort;
	int		index[3];

	index[0] = 0;
	index[1] = 0;
	index[2] = 1;
	sort = ft_init_sort(envp);
	while (sort[index[0]])
		index[0]++;
	while (sort[index[1]])
	{
		while (index[2] < index[0] - index[1] - 1)
		{
			if (ft_strncmp(sort[index[2] + 1], sort[index[2]]) < 0)
				ft_swap(&sort[index[2] + 1], &sort[index[2]]);
			index[2]++;
		}
		index[2] = 0;
		index[1]++;
	}
	sort[index[0]] = NULL;
	return (sort);
}
