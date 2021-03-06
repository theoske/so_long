# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tkempf-e <tkempf-e@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/11 00:14:01 by tkempf-e          #+#    #+#              #
#    Updated: 2022/07/11 01:01:25 by tkempf-e         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	=	errors.c get_next_line.c initialisation.c itoa.c key_hook.c main.c \
			map_checker.c map_content_checker_utils.c moving.c parser.c \
			utils.c

OBJS	=	$(join $(addsuffix ./src/, $(dir $(SRCS))), $(notdir $(SRCS:.c=.o)))

GCC 	=	gcc

RM		=	rm -f

FLAGS	=	-Wall -Wextra -Werror

NAME	=	so_long

.c.o:
	$(GCC) $(FLAGS) -c -I. $< -o ${<:.c=.o}

$(NAME):	$(OBJS)
					@$(GCC) $(FLAGS) $(OBJS) -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)

all:		$(NAME)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME) a.out

re:			fclean all

.PHONY:		all clean fclean re