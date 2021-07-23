# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: widraugr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/29 13:07:44 by widraugr          #+#    #+#              #
#    Updated: 2021/07/23 07:57:29 by mixfon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ssl

LIBDIR = ./libft

FILE_C = main.c\
		 md5.c\
		 sha256.c

FLAGS = -Wall -Wextra -Werror -I libft -I include 

FLIB = -L ./libft -lftprintf 

DIRC = ./source/

VPATH = $(DIROBJ)

DIROBJ = ./obj/

OBJ = $(addprefix $(DIROBJ), $(FILE_C:.c=.o))

all : $(NAME)

$(NAME): $(DIROBJ) $(OBJ) 
	make -C $(LIBDIR)
	gcc -g $(FLAGS) $(OBJ) $(FLIB) $(FMLXLIB) -ltermcap -o $(NAME)

$(DIROBJ)%.o : $(DIRC)%.c
	gcc -g $(FLAGS) -c $< -o $@

$(DIROBJ):
	mkdir -p $(DIROBJ)

clean:
	/bin/rm -rf $(DIROBJ)
	
fclean: clean
	/bin/rm -f $(NAME)
	/bin/rm -rf *.dSYM
	make fclean -C $(LIBDIR)
	
re: fclean all 
