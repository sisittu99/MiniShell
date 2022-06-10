#ifndef MINISHELL_H
# define MINISHELL_H

# include <termcap.h>
# include <curses.h>
# include <term.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <signal.h>
# include "../libft/libft.h"

# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_bash
{
	// struct s_bash	*prev;
	// char			*line;
	char			**cmd;
	pid_t			proc;
	int				pipe[2];
	char			sep;
	char			re_dir;
	int				built;
	char			**envp;
	struct s_bash	*next;
}				t_bash;

char	**ft_new_env(char **mat, int def);

// * SIGNALS * //

void	ft_sig_define(struct sigaction *sa, int def);
void	ft_sig_handler(int sig);
void	ft_control_d(char *line);
void	ft_sig_default(int sig);

// * READLINE * //

void	rl_replace_line(const char *text, int clear_undo);

// * PARSE * //

void	ft_parse(t_bash **bash, char *line, char **envp);
char	**ms_split(char *s);
char	*ft_replace(char *s, char *envp[], int pos, int *ret_i);
char	*ft_delete_char(char *s, int pos);
int		ms_strchr(char *s, int pos, char c);

// * EXECUTE * //

void	ft_execute(t_bash **bash, char **envp, char **line);

// * BUILTIN * //

char	**ft_builtin_assgn(void);
int		ft_check_builtin(char *cmd);
void	ft_exec_builtin(t_bash **bash, char **envp);
void	ft_echo(char **cmd);
int		ft_pwd(char **envp);
void	ft_env(char **envp);
void	ft_unset(t_bash **bash, char **cmd, char **envp);

// * EXPORT * //

void	ft_export(t_bash **bash, char **cmd, char **envp);
void	ft_env_var_found(char *cmd, t_bash **bash, int index[3]);

// * PATH * //

char	**ft_path(char **envp);
char	*ft_access(char *cmd, char **path);

// * LIST UTILS * //

t_bash	*ft_new_node(char *line, int pos, int len, char *sep);
void	ft_node_add_back(t_bash **lst, t_bash *new);
void	ft_delete_lst(t_bash **bash);
void	ft_free(char **dc);

// void	rl_replace_line (const char *text, int clear_undo);

#endif
