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
	struct s_bash	*prev;
	char			**cmd;
	int				pipe;
	char			sep;
	struct s_bash	*next;
}				t_bash;


// * PATH * //

char	**ft_path(char **envp);
char	*ft_access(char *cmd, char **path);

// * UTILS * //

char	*ft_replace(char *s, char *envp[], int pos, int *ret);
char	**ms_split(char *s, char c, char *envp[]);
char	*ft_delete_char(char *s, int pos);

#endif
