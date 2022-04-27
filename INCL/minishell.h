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
# include "libft.h"

// typedef struct s_list
// {
// 	char			**cmd;
// 	int				pipe;
// 	char			sep;
// 	struct s_list	*next;
// }				t_list;


char	*ft_replace(char *s, char *envp[]);
char	**ms_split(const char *s, char c, char *envp[]);

// * PATH * //

char	**ft_path(char **envp);
char	*ft_access(char *cmd, char **path);

#endif
