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
	new = (char **) malloc (sizeof(char *) * (i + def + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (mat[i])
	{
		new[i] = ft_strdup(mat[i]);
		i++;
	}
	new[i] = 0;
	return (new);
}

void	ft_command(t_bash **bash, struct sigaction *sa, char **envp, char **tmp)
{
	char	*line;
	char	**env;

	env = ft_new_env(envp, 0);
	while (1)
	{
		ft_sig_define(sa, 0);
		line = readline("bash-biutiful>$ ");
		if (!line)
			ft_control_d(line);
		if (*line)
		{
			if (ft_parse(bash, line, env) == 1)
			{
				printf("HERE!\n");
				if ((*bash)->next != NULL || (*bash)->re_dir == '1')
					ft_sig_define(sa, 1);
				ft_execute(bash, env, &line);
			}
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
}

int	main(int argc, char **argv, char **envp)
{
	t_bash				*bash;
	char				*tmp;		//DA METTERE IN STRUCT PERCHÉ RESTA ALLOCATA QUANDO SI ESCE DAL PROGRAMMA *** //
	struct sigaction	sa;

	(void)argv;
	if (argc != 1)
	{
		write(2, "error: no argument allowed\n", 28);
		return (1);
	}
	bash = NULL;
	tmp = NULL;
	ft_command(&bash, &sa, envp, &tmp);
	free(tmp); //*** LEAKS ! (NON CI ARRIVERÀ MAI)//
	return (exit_status);
}


/* 1) sistemare il find it
   2) sistemare syntax error pipe/redir/ecc.
   3) gestire lo status di uscita delle builtin
   4) controllare gli errori delle altre e i loro messaggi
   5) pulizia, controlli che poi si comincia coi bonus belli a papà
   6) leaks */
