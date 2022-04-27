# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/22 15:36:33 by fdrudi            #+#    #+#              #
#    Updated: 2022/04/27 18:41:57 by fdrudi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

SRC			=	SRC/main.c SRC/path.c SRC/ms_split.c SRC/ms_substr.c SRC/ft_replace\

OBJ			=	$(SRC:.c=.o)

OBJ_DIR		=	$(./OBJ)

LIBFT		=	./INCL/libft.a

CC			=	gcc -lreadline -ltermcap

RM			=	rm -f

CFLAGS		=	#-Wall -Wextra -Werror

%.o:%.c
			$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(OBJ)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT)

all:		$(NAME)

clean:
			${RM} $(OBJ)

fclean: 	clean
			${RM} $(NAME) ${OBJ}

re:			fclean all

.PHONY:		all clean fclean re
