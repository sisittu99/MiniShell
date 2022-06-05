	#include "./INCL/minishell.h"

int	main()
{
	t_bash *bash;
	int		i[3] = {1, 3, 3};

	bash->envp = (char **) malloc (sizeof(char *) * 5);
	bash->envp[0] = ft_strdup("uno=ciao");
	bash->envp[1] = ft_strdup("due=ciao");
	bash->envp[2] = ft_strdup("tre=ciao");
	bash->envp[3] = ft_strdup("quattro=ciao");
	ft_env_var_found("uno=fuck", &bash, i);
	int ind = -1;
	while (++ind < 5)
		printf("%s\n", bash->envp[ind]);
	return (0);
}
