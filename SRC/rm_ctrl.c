/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rm_ctrl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcerchi <mcerchi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 18:46:05 by mcerchi           #+#    #+#             */
/*   Updated: 2022/06/22 18:46:06 by mcerchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/*
	inizializza le stringe per ft_rm_ctrl(), rispettando i free().
*/
void	ft_init_rm_ctrl(char **tmp, char **command, char **envp)
{
	char	*tilde;
	char	*home;
	int		i;

	i = 0;
	tilde = ft_strdup("~");
	home = ft_replace_tilde(tilde, envp, 0, &i);
	*command = ft_strdup("set echo-control-characters Off\n");
	*tmp = ft_strjoin(home, "/.inputrc");
	free(home);
}

/*
	Crea o modifica il file ~/.inputrc,
	nel quale si spegne la stampa dei caratteri di controllo.
*/
void	ft_rm_ctrl(char **envp)
{
	int		fd;
	char	*tmp;
	char	*command;

	ft_init_rm_ctrl(&tmp, &command, envp);
	fd = open(tmp, O_RDWR | O_CREAT | O_APPEND, 0777);
	free(tmp);
	tmp = ft_get_next_line(fd);
	while (tmp)
	{
		if (ft_strcmp(tmp, command) == 1)
		{
			close(fd);
			free(tmp);
			free(command);
			return ;
		}
		tmp = ft_get_next_line(fd);
	}
	fd_printf(fd, "set echo-control-characters Off\n");
	close(fd);
	free(tmp);
	free(command);
}
