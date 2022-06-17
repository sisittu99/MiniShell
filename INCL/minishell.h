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
# include <dirent.h>
# include "../libft/libft.h"

# include <sys/types.h>
# include <sys/wait.h>

// * COLORS * //

# ifndef COLORS_H
#  define COLORS_H
#  define RESET   "\033[0m"
#  define BLACK   "\033[30m"      /* Black */
#  define RED     "\033[31m"      /* Red */
#  define GREEN   "\033[32m"      /* Green */
#  define YELLOW  "\033[33m"      /* Yellow */
#  define BLUE    "\033[34m"      /* Blue */
#  define MAGENTA "\033[35m"      /* Magenta */
#  define CYAN    "\033[36m"      /* Cyan */
#  define WHITE   "\033[37m"      /* White */
#  define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#  define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#  define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#  define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#  define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#  define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#  define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#  define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
# endif

typedef struct s_bash
{
	// struct s_bash	*prev;
	char			*line;
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
char	**wd_split(const char *s, char c);

// * GLOBAL VARIABLE * //

int		exit_status;

// * SIGNALS * //

void	ft_sig_define(struct sigaction *sa, int def);
void	ft_sig_handler(int sig);
void	ft_control_d(char *line);
void	ft_sig_default(int sig);

// * READLINE * //

void	rl_replace_line(const char *text, int clear_undo);

// * PARSE * //

int		ft_parse(t_bash **bash, char *line, char **envp);
char	**ms_split(char *s);
void	ft_replace(char **s, char **envp, int pos, int *ret_i);
char	*ft_delete_char(char *s, int pos);
int		ms_strchr(char *s, int pos, char c);

// * EXECUTE * //

void	ft_execute(t_bash **bash, char **envp, char **line);

// * BUILTIN * //

char	**ft_builtin_assgn(void);
int		ft_check_builtin(char *cmd);
void	ft_exec_builtin(t_bash **bash, char **envp, int def);
int		ft_echo(char **cmd);
int		ft_pwd(char **cmd, char **envp);
int		ft_cd(t_bash **bash, char **cmd, char **envp);
int		ft_env(char **cmd, char **envp);
int		ft_unset(t_bash **bash, char **cmd, char **envp);
void	ft_exit(char **cmd);
int		ft_invalid_option(char **cmd);

// * EXPORT * //

int		ft_export(t_bash **bash, char **cmd, char **envp);
void	ft_env_var_found(char *cmd, t_bash **bash, int index[3]);

// * PATH * //

char	**ft_path(char **envp);
char	*ft_access(char *cmd, char **path);
int		*find_it(char **envp, char *to_find);

// * LIST UTILS * //

t_bash	*ft_new_node(char *line, int pos, int len, char *sep);
void	ft_node_add_back(t_bash **lst, t_bash *new);
void	ft_delete_lst(t_bash **bash);
void	ft_free(char **dc);

// void	rl_replace_line (const char *text, int clear_undo);

#endif
