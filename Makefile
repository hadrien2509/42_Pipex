# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/19 14:22:07 by hgeissle          #+#    #+#              #
#    Updated: 2023/03/13 13:22:40 by hgeissle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

SRC = src/pipex.c src/errors.c src/libft_fcts.c src/child.c

SRC_LIBFT = ft_printf/ft_printf.c ft_printf/ft_printf_tools.c src/ft_split.c src/ft_strjoin.c src/ft_strncmp.c

FLAGS = -Wall -Wextra -Werror
INCLUDE = 
AR = ar rcs
RM = rm -f
OBJS = $(SRC:.c=.o)
OBJS_LIBFT = $(SRC_LIBFT:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(OBJS_LIBFT)
	cc $(FLAGS) $(OBJS) $(OBJS_LIBFT) $(INCLUDE) -o $(NAME)

clean:
		$(RM) $(OBJS) $(OBJS_LIBFT)

fclean: clean
		$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re