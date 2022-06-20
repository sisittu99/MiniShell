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

void	ft_command(t_bash **bash, struct sigaction *sa, char **envp)
{
	char	*line;
	char	**env;

	env = ft_new_env(envp, 0);
	while (1)
	{
		ft_sig_define(sa, 0);
		if (exit_status != 0)
			line = readline(BOLDRED"bash-st00pid>$ "RESET);
		else
			line = readline(BOLDGREEN"bash-biutiful>$ "RESET);
		if (!line)
			ft_control_d(line);
		if (*line)
		{
			if (ft_parse(bash, line, env) == 1)
			{
				if ((*bash)->next != NULL || (*bash)->re_dir == '1')
					ft_sig_define(sa, 1);
				ft_execute(bash, env, &line);
			}
			add_history(line);
			if (*bash && (*bash)->envp)
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
	struct sigaction	sa;

	(void)argv;
	if (argc != 1)
	{
		write(2, "error: no argument allowed\n", 28);
		return (1);
	}
	bash = NULL;
	ft_rm_ctrl(envp);
	ft_command(&bash, &sa, envp);
	return (exit_status);
}


/* 1) sistemare il find it.
   5) pulizia, controlli che poi si comincia coi bonus belli a papà
   6) leaks */
