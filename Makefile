# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fdrudi <fdrudi@student.42roma.it>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/22 15:36:33 by fdrudi            #+#    #+#              #
#    Updated: 2022/04/28 13:06:08 by fdrudi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

SRC			=	SRC/main.c SRC/path.c SRC/ms_split.c SRC/ft_replace\

OBJ			=	$(SRC:.c=.o)

LIBFT		=	INCL/libft/libft.a

CC			=	gcc -lreadline #-ltermcap

RM			=	rm -f

CFLAGS		=	#-Wall -Wextra -Werror

%.o:%.c
			$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(LIBFT) $(OBJ)
			$(CC) $(CFLAGS) $(LIBFT) $(OBJ) -o $(NAME)

$(LIBFT):	$(LIBFT)
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
