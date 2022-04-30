#ifndef MINISHELL_H
# define MINISHELL_H

# include <termcap.h>
# include <curses.h>
# include <term.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include "../libft/libft.h"

typedef struct s_bash
{
	// struct s_bash	*prev;
	char			*line;
	char			**cmd;
	int				pipe;
	char			sep;
	char			re_dir[2];
	struct s_bash	*next;
}				t_bash;

// * PARSE * //

void	ft_parse(t_bash **bash, char *line, char **envp);
char	**ms_split(char *s);
char	*ft_replace(char *s, char *envp[], int pos, int *ret_i);
char	*ft_delete_char(char *s, int pos);
int		ms_strchr(char *s, int pos, char c);

// * PATH * //

char	**ft_path(char **envp);
char	*ft_access(char *cmd, char **path);

// * LIST UTILS * //

t_bash	*ft_new_node(char *line, int pos, int len, char *sep);
void	ft_node_add_back(t_bash **lst, t_bash *new);
void	ft_delete_lst(t_bash **bash);

#endif
