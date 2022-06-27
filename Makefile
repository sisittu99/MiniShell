# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcerchi <mcerchi@student.42roma.it>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/22 15:36:33 by fdrudi            #+#    #+#              #
#    Updated: 2022/06/27 11:48:36 by mcerchi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

SRC			=	./SRC/main.c ./SRC/path.c ./SRC/ms_split.c \
				./SRC/ms_split_utils.c ./SRC/ft_replace.c \
				./SRC/parse.c ./SRC/list_utils.c ./SRC/execute.c \
				./SRC/builtin/check_builtin.c ./SRC/builtin/builtin.c \
				./SRC/builtin/echo.c ./SRC/builtin/chdir.c \
				./SRC/builtin/export.c ./SRC/builtin/export_one.c \
				./SRC/builtin/export_two.c \
				./SRC/builtin/unset.c ./SRC/signals.c ./SRC/rm_ctrl.c\
				./SRC/wd_split.c ./SRC/wildcard.c ./SRC/wildcard_help.c \
				./SRC/re_dir.c ./SRC/ft_pipe.c ./SRC/ft_and_or.c \
				./SRC/execute_b.c ./SRC/ft_replace_b.c ./SRC/syntax_error.c \
				./SRC/ft_find.c ./SRC/ft_find_utils.c ./SRC/ft_par.c\

OBJ			=	$(SRC:.c=.o)

LIBFT		=	libft/libft.a

READLINE	=	-L/usr/include -lreadline -L$(HOME)/.brew/opt/readline/lib \
				-I$(HOME)/.brew/opt/readline/include\

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

run:		all
			@./minishell

.PHONY:		all clean fclean re
