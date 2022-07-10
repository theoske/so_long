# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/11 00:14:01 by tkempf-e          #+#    #+#              #
#    Updated: 2022/07/11 00:20:27 by tkempf-e         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	=	

OBJS	=	$(addsuffix ./src/), $(SRCS:.c=.o)

GCC 	=	gcc

RM		=	rm -f

FLAGS	=	-Wall -Werror -Wextra -lmlx -framework OpenGL -framework AppKit

NAME	=	so_long

.c.o:
	$(GCC) $(FLAGS) -c -I. $< -o ${<:.c=.o}

$(NAME):	$(OBJS)

all:		$(NAME)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME) a.out

re:			fclean all

.PHONY:		all clean fclean re