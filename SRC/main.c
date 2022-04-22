#include "../INCL/minishell.h"

int	main()
{
	// char	*term;
	// char	buffer[2048];
	// int		get_term;

	// term = getenv("TERM");
	// printf("Term : %s\n", term);
	// get_term = tgetent(buffer, term);
	// return (0);
  const char *name;
  char  *bp;
  char  *term;
  int   height;
  int   width;

  bp = malloc(sizeof(*bp));
  name = "TERM";
  if ((term = getenv(name)) == NULL)
    return (1);
  printf("My terminal is %s.\n", term);
  tgetent(bp, term);
  height = tgetnum ("li");
  width = tgetnum ("co");
  printf("H : %d\nL : %d\n", height, width);
  free(bp);
  return (0);
}
