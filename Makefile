# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/19 14:22:07 by hgeissle          #+#    #+#              #
#    Updated: 2023/03/17 14:20:45 by hgeissle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

SRC = mandatory/pipex.c mandatory/errors.c mandatory/child.c

SRC_LIBFT = ft_printf/ft_printf.c ft_printf/ft_printf_tools.c mandatory/ft_split.c mandatory/ft_strjoin.c mandatory/ft_strncmp.c mandatory/libft_fcts.c

SRC_BONUS = bonus/pipex_bonus.c bonus/child_bonus.c

FLAGS = -Wall -Wextra -Werror
INCLUDE = -fsanitize=address
AR = ar rcs
RM = rm -f
OBJS = $(SRC:.c=.o)
OBJS_LIBFT = $(SRC_LIBFT:.c=.o)
OBJS_BONUS = $(SRC_BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(OBJS_LIBFT)
	cc $(FLAGS) $(OBJS) $(OBJS_LIBFT) $(INCLUDE) -o $(NAME)

clean:
		$(RM) $(OBJS) $(OBJS_LIBFT) $(OBJS_BONUS)

fclean: clean
		$(RM) $(NAME)
		
bonus : $(OBJS_BONUS) $(OBJS_LIBFT)
	cc $(FLAGS) $(OBJS_BONUS) $(OBJS_LIBFT) $(INCLUDE) -o $(NAME)

re: fclean all

rebonus : fclean bonus

.PHONY: all clean fclean bonus re rebonus