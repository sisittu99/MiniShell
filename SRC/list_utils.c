#include "../INCL/minishell.h"

void	ft_delete_lst(t_bash **bash)
{
	t_bash	*tmp;

	if (*bash== NULL)
		return ;
	tmp = *bash;
	while (*bash!= NULL)
	{
		tmp = (*bash)->next;
		free (*bash);
		*bash= tmp;
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
	t_bash	*new;
	int		i;

	new = (t_bash *) malloc (sizeof(t_bash));
	new->line = (char *) malloc (sizeof(char) * len + 1);
	if (!new || !new->line)
		return (NULL);
	i = 0;
	while (i < len)
		new->line[i++] = line[pos++];
	new->line[i] = '\0';
	new->cmd = NULL;
	if (sep[1] != '0')
		new->sep = sep[0];
	else if (sep[0] != '0')										// DA SISTEMARE E PREDISPORRE PER '>' '<' ECC.. //
		new->pipe = 1;
	else
		new->pipe = 0;
	new->next = NULL;
	return (new);
}
