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

// * STRUCT * //

typedef struct s_bash
{
	char			*line;
	char			**cmd;
	pid_t			proc;
	int				pipe[2];
	char			sep;
	char			re_dir;
	int				built;
	int				par;
	int				id;
	char			**envp;
	struct s_bash	*next;
}				t_bash;

// * GLOBAL VARIABLE * //

int		g_exit_status;

// * WILDCARD * //

char	*ft_wildcard(char *s, char *pwd, int pos, int *ret_i);
char	*ft_find_wildcard(char *s, int *pos);
int		ft_check_wildcard(char **wild, char *name);
char	**wd_split(const char *s, char c);
int		wd_strncmp(char **s1, char *s2, int *pos, int j);

// * SIGNALS * //

void	ft_sig_define(struct sigaction *sa, int def);
void	ft_sig_handler(int sig);
void	ft_control_d(char *line);
void	ft_sig_default(int sig);
void	ft_rm_ctrl(char **envp);

// * READLINE * //

void	rl_replace_line(const char *text, int clear_undo);

// * PARSE * //

int		ft_parse(t_bash **bash, char *line, char **envp);
void	ft_replace(char *s, char **envp, int pos, int *ret_i);
char	*ft_replace_help(char *s, char **envp, int pos, int *ret_i);
char	*ft_replace_help_b(char *s, char **envp, int *ret_i, int *i);
char	*ft_replace_tilde(char *s, char **envp, int pos, int *ret_i);
int		ft_check_var(char *s, int pos);
char	*ft_replace_join(char *s1, char *s2, char *s3);
char	*ft_delete_char(char *s, int pos);
int		ms_strchr(char *s, int pos, char c);
	//ms_split//
char	**ms_split(char *s);
int		ms_nb_words(char *s, char c);
void	nb_words_help(char *s, int a, int *i, int *j);
void	nb_words_redir(char *s, int a, int *j);
void	ms_wds_assign(char *s, char c, char **dest, int len);
void	wds_assign_help(char *s, int *i, int len);
void	wds_assign_help_b(char *s, int *i, char **dest, int *x);

// * EXECUTE * //

int		ft_execute(t_bash **bash, char **envp, char **line);
int		ft_check_exec(t_bash **tmp, char **envp, char *line);
int		ft_check_exec_help(t_bash **tmp, char **envp, char *line, int *def);
void	ft_check_new_cmd(t_bash **bash, char **cpy, char **envp);
int		ft_lonely_cmd(t_bash **bash, char **envp, char *line);
void	ft_execve(t_bash **bash, char **envp, char *line, int def);
char	**ft_delete_cmd(char **cmd, int pos);
	//and_or//
int		ft_and_or(t_bash **bash, char **envp, char *line, int *def);
int		ft_and(t_bash **bash, int *def, int *par);
int		ft_or(t_bash **bash, int *def, int *par);
	//re_dir//
int		ft_check_re_dir(t_bash **bash, int i, char *line);
int		ft_re_dir_help(t_bash **bash, int i, int fd);
int		ft_re_dir_help_b(t_bash **bash, int i, char *line);
void	ft_re_dir_loop(t_bash **bash, int i, int *pip, char *line);
	//pipe//
void	ft_pipe(t_bash **bash, char **envp, char *line);
void	ft_pipe_help(t_bash **bash, t_bash **start, char **envp, char *line);
t_bash	*ft_pipe_help_b(t_bash **bash, t_bash **start, char **envp, char *line);
void	ft_close_pipe(t_bash **bash);

// * BUILTIN * //

char	**ft_builtin_assgn(void);
int		ft_check_builtin(char *cmd);
void	ft_exec_builtin(t_bash **bash, char **envp, int def);
int		ft_invalid_option(char **cmd);
	//echo//
int		ft_echo(char **cmd);
	//pwd//
int		ft_pwd(char **cmd, char **envp);
	//cd//
int		ft_cd(t_bash **bash, char **cmd, char **envp);
	//env//
int		ft_env(char **cmd, char **envp);
	//unset//
int		ft_unset(t_bash **bash, char **cmd, char **envp);
	//exit//
int		ft_exit(char **cmd);
	//export//
int		ft_export(t_bash **bash, char **cmd, char **envp);
void	ft_handle_env(char *cmd, t_bash **bash);
void	ft_env_var_found(char *cmd, t_bash **bash, int index[3]);
char	**ft_sort_env(char **envp);

// * PATH * //

char	**ft_path(char **envp);
char	*ft_access(char *cmd, char **path);
int		*find_it(char **envp, char *to_find);

// * UTILS * //

char	**ft_new_env(char **mat, int def);
char	*ft_prompt(void);
void	ft_free(char **dc);

// * LIST UTILS * //

t_bash	*ft_new_node(char *line, int pos, int len, char *sep);
void	ft_node_add_back(t_bash **lst, t_bash *new);
void	ft_delete_lst(t_bash **bash);
t_bash	*ms_init_node(t_bash **tmp);

// void	rl_replace_line (const char *text, int clear_undo);

#endif
