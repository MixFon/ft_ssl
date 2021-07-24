# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: widraugr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/29 13:07:44 by widraugr          #+#    #+#              #
#    Updated: 2021/07/24 11:57:30 by mixfon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ssl

LIBDIR = ./libft

FILE_C = add_string_to_array.c\
		 chenge_endian.c\
		 fill_flags.c\
		 init_ssl.c\
		 working_files.c\
		 working_stdin.c

FILE_MD5 = fill_arr_k.c\
		   rounds.c\
		   stage_one.c

FILE_SHA256 = alg_sha256.c\
			  init_arr_k_sha256.c\
			  mix_data.c

FLAGS = -Wall -Wextra -Werror -I libft -I include 

FLIB = -L ./libft -lftprintf 

DIRC = ./source/

DIRMD5 = ./md5/

DIRSHA256 = ./sha256/

DIROBJ = ./obj/

OBJ = $(addprefix $(DIROBJ), $(FILE_C:.c=.o))

OBJMD5 = $(addprefix $(DIROBJ), $(FILE_MD5:.c=.o))

OBJSHA256 = $(addprefix $(DIROBJ), $(FILE_SHA256:.c=.o))

all : $(NAME)

$(NAME): $(DIROBJ) $(OBJ) $(OBJMD5) $(OBJSHA256)
	make -C $(LIBDIR)
	gcc $(FLAGS) $(OBJ) $(OBJMD5) $(OBJSHA256) $(FLIB) -o $(NAME)

$(DIROBJ)%.o : $(DIRC)%.c
	gcc -g $(FLAGS) -c $< -o $@

$(DIROBJ)%.o : $(DIRMD5)%.c
	gcc -g $(FLAGS) -c $< -o $@

$(DIROBJ)%.o : $(DIRSHA256)%.c
	gcc -g $(FLAGS) -c $< -o $@

$(DIROBJ):
	mkdir -p $(DIROBJ)

clean:
	/bin/rm -rf $(DIROBJ)
	make clean -C $(LIBDIR)
	
fclean: clean
	/bin/rm -f $(NAME)
	/bin/rm -rf *.dSYM
	make fclean -C $(LIBDIR)
	
re: fclean all 

