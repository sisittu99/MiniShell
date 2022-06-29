/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_par.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:37:31 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/29 17:04:07 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

int	ft_nbr_par(char *line)
{
	int		i;
	int		j;
	int		pos[2];
	char	*tmp;

	i = 0;
	tmp = NULL;
	j = (int) ft_strlen(line) - 1;
	while (line[i] == '(')
		i++;
	while (line[j] == ')')
		j--;
	if (ft_par_error(line, i, j) != 0)
		return (300);
	pos[0] = i;
	pos[1] = ft_strlen(line) - 1 - j;
	if (i > 0 && j < ((int) ft_strlen(line) - 1))
		tmp = ft_substr(line, i, j);
	else
		tmp = ft_substr(line, i, j + 1);
	free(line);
	line = ft_strdup(tmp);
	free(tmp);
	return (pos[0] - pos[1]);
}

int	ft_find_par(t_bash **bash)
{
	t_bash	*tmp;
	int		lvl[2];

	tmp = *bash;
	lvl[0] = ft_nbr_par(tmp->line);
	if (lvl[0] < 0)
		return (ft_par_error_b(tmp->par, lvl[0]));
	else if (lvl[0] == 300)
		return (0);
	tmp->par = lvl[0];
	while (tmp->next != NULL)
	{
		lvl[1] = ft_nbr_par(tmp->next->line);
		if (lvl[1] == 300)
			return (0);
		if (lvl[1] >= 0)
			tmp->next->par = tmp->par + lvl[1];
		else
			tmp->next->par = tmp->par;
		if (lvl[0] < 0)
			tmp->next->par += lvl[0];
		tmp = tmp->next;
		lvl[0] = lvl[1];
	}
	if (tmp->par + lvl[0] != 0)
		return (ft_par_error_b(tmp->par, lvl[0]));
	return (1);
}
