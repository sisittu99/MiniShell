/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 17:13:37 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/21 17:13:37 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

char	**ft_new_env(char **mat, int def)
{
	char	**new;
	int		i;

	if (!mat)
		return (NULL);
	i = 0;
	while (mat[i])
		i++;
	new = (char **) malloc (sizeof(char *) * (i + def + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (mat[i])
	{
		new[i] = ft_strdup(mat[i]);
		i++;
	}
	new[i] = NULL;
	return (new);
}

char	*ft_prompt(t_bash **bash, char **envp)
{
	char	*line;
	int		i;

	i = 0;
	if (g_exit_status != 0)
		line = readline(BOLDRED"bash-st00pid>$ "RESET);
	else
		line = readline(BOLDGREEN"bash-biutiful>$ "RESET);
	if (!line)
		ft_control_d(line, envp, bash);
	while (line[i] == ' ')
		i++;
	if (line[i] == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}

void	ft_check_env(t_bash **bash, char ***env)
{
	t_bash	*tmp;

	tmp = *bash;
	while (tmp)
	{
		if (tmp->envp != NULL)
		{
			if (*env)
				ft_free(*env);
			(*env) = ft_new_env(tmp->envp, 0);
			if (tmp->envp)
				ft_free(tmp->envp);
			tmp->envp = NULL;
		}
		tmp = tmp->next;
	}
	tmp = NULL;
}

void	ft_command(t_bash **bash, struct sigaction *sa, char **envp)
{
	char	*line;
	char	**env;

	env = ft_new_env(envp, 0);
	while (1)
	{
		ft_sig_define(sa, 0);
		line = ft_prompt(bash, env);
		if (line != NULL)
		{
			if (ft_parse(bash, ft_strdup(line), env) == 1)
			{
				if ((*bash)->next != NULL || (*bash)->re_dir == '1')
					ft_sig_define(sa, 1);
				ft_execute(bash, env, line);
			}
			if (*bash)
				add_history((*bash)->new_line);
			else
				add_history(line);
			free(line);
		}
		ft_check_env(bash, &env);
		ft_delete_lst(bash);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_bash				*bash;
	struct sigaction	sa;
	int					i;

	i = 0;
	if (argc != 1)
	{
		fd_printf(2, "error: no arguments allowed:\n");
		while (++i < argc)
			fd_printf(2, "\"%s\"\n", argv[i]);
		return (1);
	}
	bash = NULL;
	ft_rm_ctrl(envp);
	ft_command(&bash, &sa, envp);
	return (g_exit_status);
}
