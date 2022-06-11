#include "../INCL/minishell.h"

void	ft_free(char **dc)
{
	int	i;

	i = -1;
	if (dc[0])
	{
		while (dc[++i])
			free(dc[i]);
	}
	if (dc)
		free(dc);
}

char	**ft_new_env(char **mat, int def)
{
	char	**new;
	int		i;

	if (!mat)
		return (NULL);
	i = 0;
	while (mat[i])
		i++;
	new = (char **) malloc (sizeof(char *) * i + def);
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

void	ft_command(t_bash **bash, struct sigaction *sa, char **env, char **tmp)
{
	char	*line;

	line = readline("bash-biutiful>$ ");
	if (!line)
		ft_control_d(line);
	if (*line)
	{
		ft_parse(bash, line, env);
		if ((*bash)->next != NULL || (*bash)->re_dir == '1')
			ft_sig_define(sa, 1);
		ft_execute(bash, env, &line);
		if (*tmp == NULL || ft_strcmp(*tmp, line) == 0)
		{
			add_history(line);
			if (*tmp)
				free(*tmp);
			*tmp = ft_strdup(line);
		}
		if ((*bash)->envp)
		{
			ft_free(env);
			env = ft_new_env((*bash)->envp, 0);
		}
		ft_delete_lst(bash);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_bash				*bash;
	char				*tmp;		//DA METTERE IN STRUCT PERCHÉ RESTA ALLOCATA QUANDO SI ESCE DAL PROGRAMMA *** //
	char				**env;
	struct sigaction	sa;

	(void)argv;
	if (argc != 1)
	{
		write(2, "error: no argument allowed\n", 28);
		return (1);
	}
	bash = NULL;
	tmp = NULL;
	env = ft_new_env(envp, 0);
	while (1)
	{
		ft_sig_define(&sa, 0);
		ft_command(&bash, &sa, env, &tmp);
	}
	free(tmp); //*** LEAKS ! (NON CI ARRIVERÀ MAI)//
	return (0);
}
