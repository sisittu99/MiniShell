#include "../INCL/minishell.h"

void	ft_delete_lst(t_bash **bash)
{
	t_bash	*tmp;

	if (*bash == NULL)
		return ;
	tmp = *bash;
	while (*bash != NULL)
	{
		tmp = (*bash)->next;
		if ((*bash)->line[0] != '\0')
			free((*bash)->line);
		if ((*bash)->cmd != NULL)
			ft_free((*bash)->cmd);
		free (*bash);
		*bash = tmp;
	}
}

void	ft_lst_delete(t_list **stack)
{
	t_list	*tmp;

	if (*stack == NULL)
		return ;
	tmp = *stack;
	while (*stack != NULL)
	{
		tmp = (*stack)->next;
		free (*stack);
		*stack = tmp;
	}
}

t_bash	*ft_last_node(t_bash *lst)
{
	while (lst != NULL)
	{
		if (lst->next == NULL)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

void	ft_node_add_back(t_bash **lst, t_bash *new)
{
	t_bash	*tmp;

	if (*lst != NULL)
	{
		tmp = ft_last_node(*lst);
		tmp->next = new;
	}
	else
		(*lst) = new;
}

t_bash	*ft_new_node(char *line, int pos, int len, char *sep)
{
	t_bash		*new;
	char		*tmp;
	static int	id;

	new = (t_bash *) malloc (sizeof(t_bash));
	tmp = ft_substr(line, pos, len);
	new->line = ft_strtrim(tmp, " ");
	free(tmp);
	new->cmd = NULL;
	new->sep = sep[1];
	if ((sep[0] - 48) == 1)
	{
		if (pipe(new->pipe) == -1)
		{
			write(2, "error: could not create pipe\n", 32);
			exit(errno);
		}
	}
	else
	{
		new->pipe[0] = 0;
		new->pipe[1] = 0;
	}
	new->re_dir = sep[2];
	new->built = -2;
	new->par = 0;
	new->id = id;
	id += 1;
	new->envp = NULL;
	new->next = NULL;
	return (new);
}
