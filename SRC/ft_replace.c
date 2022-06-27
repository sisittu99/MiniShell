/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:57:51 by fdrudi            #+#    #+#             */
/*   Updated: 2022/06/21 16:57:51 by fdrudi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../INCL/minishell.h"

/* -> Controlla che la presunta variabile sia scritta
	  secondo le regole grammaticali corrette <- */
int	ft_check_var(char *s, int pos)
{
	int	i;

	i = 0;
	if (s[pos] == 63 && (s[pos + 1] == 32 || s[pos + 1] == '\0'))
		return (-1);
	if (ft_isalpha(s[pos]) || s[pos] == 95)
	{
		while (s[pos + i] != '\0' && (ft_isalpha(s[pos + i])
				|| ft_isdigit(s[pos + i]) || s[pos + i] == 95))
			i++;
		return (i);
	}
	return (0);
}

/* -> Funzione che sostituisce la tilde con la path HOME <-*/
char	*ft_replace_tilde(char *s, char **envp, int pos, int *ret_i)
{
	char	*s1;
	char	*s2;
	char	*s3;
	int		*index;
	int		i;

	s1 = ft_substr(s, 0, pos);
	index = find_it(envp, "HOME");
	i = pos + 1;
	if (index != NULL)
	{
		s2 = ft_substr(envp[index[0]], index[1] + 1,
				ft_strlen(envp[index[0]]));
		*ret_i = pos + ft_strlen(s2);
	}
	else
		s2 = ft_strdup("~");
	if (index)
		free(index);
	s3 = ft_substr(s, i, (ft_strlen(s) - i));
	free(s);
	s = ft_replace_join(s1, s2, s3);
	return (s);
}

char	*ft_replace_help_b(char *s, char **envp, int *ret_i, int *i)
{
	char	*var;
	char	*s2;
	char	*s3;
	int		*index;

	var = ft_substr(s, (i[2] + 1), i[1]);
	i[0] = i[2] + 1 + i[1];
	index = find_it(envp, var);
	if (index != NULL)
	{
		s2 = ft_substr(envp[index[0]], index[1] + 1,
				ft_strlen(envp[index[0]]));
	}
	else
		s2 = /*ft_strjoin("$", var);*/ "\0";
	free(var);
	*ret_i = i[2] + ft_strlen(s2);
	var = ft_substr(s, i[0], (ft_strlen(s) - i[0]));
	s3 = ft_strjoin(s2, var);
	free(var);
	if (s2[0] != '\0')
		free(s2);
	return (s3);
}

/* -> Funzione helper per la funzione Replace <- */
char	*ft_replace_help(char *s, char **envp, int pos, int *ret_i)
{
	char	*s1;
	char	*s2;
	char	*s3;
	char	*tmp;
	int		i[3];

	ft_init_replace(&s1, &s2, &s3);
	tmp = ft_strdup(s);
	ft_init_arr_replace(i, pos, tmp);
	if (i[1] > 0 || i[1] == -1)
	{
		printf("Hello world\n");
		s1 = ft_substr(tmp, 0, pos);
		if (i[1] == -1)
		{
			s2 = ft_itoa(g_exit_status);
			i[0] = pos + 1 + 1;
			*ret_i = pos + ft_strlen(s2);
			s3 = ft_substr(s, i[0], (ft_strlen(tmp) - i[0]));
		}
		else if (i[1] > 0)
			s3 = ft_replace_help_b(tmp, envp, ret_i, i);
		free(tmp);
		tmp = ft_replace_join(s1, s2, s3);
	}
	return (tmp);
}

/* -> Controllo e ricerca della variabile all'interno della srtinga.
      Se la variabile è presente, suddivisione della stringa in 3 e
	  sostituzione della variabile con il suo valore.
	  || ** Questa funzione dev'essere chiamata dopo il controllo e la
	  conferma che la variabile dev'essere sostituita col valore ** || <- */
void	ft_replace(char **s, char **envp, int pos, int *ret_i)
{
	char	*tmp;
	char	pwd[256];

	/*	DOUBLE FREE DETECHED	*/
	printf("entered\n");
	tmp = ft_strdup(*s);
	free(*s);
	// exit(0);
	if (tmp[pos] == '*')
	{
		getcwd(pwd, sizeof(pwd));
		(*s) = ft_wildcard(tmp, pwd, pos, ret_i);
	}
	else if (tmp[pos] == '~')
		(*s) = ft_replace_tilde(ft_strdup(tmp), envp, pos, ret_i);
	else if (tmp[pos + 1] == '\0')
	{
		(*s) = ft_strdup(tmp);
		free(tmp);
		return ;
	}
	else if (tmp[pos] == '$' && ((*s)[pos + 1] != '\"'))
		(*s) = ft_replace_help(tmp, envp, pos, ret_i);
	if (tmp != NULL)
		free(tmp);
	return ;
}
