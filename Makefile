# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/22 15:36:33 by fdrudi            #+#    #+#              #
#    Updated: 2022/06/13 12:31:42 by fdrudi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

SRC			=	./SRC/main.c ./SRC/path.c ./SRC/ms_split.c ./SRC/ft_replace.c\
				./SRC/parse.c ./SRC/list_utils.c ./SRC/execute.c\
				./SRC/builtin/check_builtin.c ./SRC/builtin/builtin.c \
				./SRC/builtin/echo.c ./SRC/builtin/chdir.c \
				./SRC/builtin/export.c ./SRC/builtin/unset.c ./SRC/signals.c\

OBJ			=	$(SRC:.c=.o)

LIBFT		=	libft/libft.a

READLINE	=	-L/usr/include -lreadline -L$(HOME)/.brew/opt/readline/lib -I$(HOME)/.brew/opt/readline/include

CC			=	gcc #-fsanitize=address

RM			=	rm -f

CFLAGS		=	-Wall -Wextra -Werror

%.o:%.c
			$(CC) -g $(CFLAGS) -c $< -o $@

$(NAME):	$(LIBFT) $(OBJ)
			$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) $(READLINE)
$(LIBFT):
			make -C ./libft

all:		 $(NAME)

clean:
			${RM} $(OBJ)

fclean: 	clean
			${RM} $(NAME) ${OBJ}

fclean.all:	fclean
			make fclean -C ./libft

re:			fclean all

re.all:		fclean.all all

norm:
			@norminette

.PHONY:		all clean fclean re
