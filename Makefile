# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/22 15:36:33 by fdrudi            #+#    #+#              #
#    Updated: 2022/04/29 13:51:41 by fdrudi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

SRC			=	./SRC/main.c ./SRC/path.c ./SRC/ms_split.c ./SRC/ft_replace.c\

OBJ			=	$(SRC:.c=.o)

LIBFT		=	libft/libft.a

CC			=	gcc #-fsanitize=address

RM			=	rm -f

CFLAGS		=	-Wall -Wextra -Werror

%.o:%.c
			$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(LIBFT) $(OBJ)
			$(CC) $(CFLAGS) -lreadline -ltermcap $(OBJ) $(LIBFT) -o $(NAME)

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

.PHONY:		all clean fclean re
