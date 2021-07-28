# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: widraugr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/29 13:07:44 by widraugr          #+#    #+#              #
#    Updated: 2021/07/28 11:03:02 by mixfon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ssl

LIBDIR = ./libft

FILE_C = add_string_to_array.c\
		 chenge_endian.c\
		 fill_flags.c\
		 init_ssl.c\
		 working_files.c\
		 type_hashing_algorithms.c\
		 working_stdin.c

FILE_MD5 = fill_arr_k.c\
		   rounds.c\
		   stage_one.c

FILE_SHA256 = alg_sha256.c\
			  init_arr_k_sha256.c\
			  mix_data.c

FILE_BASE64 = base64.c

#FLAGS = -g -Wall -Wextra -Werror -I libft -I include 
FLAGS = -g -I libft -I include 

FLIB = -L ./libft -lftprintf 

DIRC = ./source/

DIRMD5 = ./md5/

DIRSHA256 = ./sha256/

DIRBASE64 = ./base64/

DIROBJ = ./obj/

OBJ = $(addprefix $(DIROBJ), $(FILE_C:.c=.o))

OBJMD5 = $(addprefix $(DIROBJ), $(FILE_MD5:.c=.o))

OBJSHA256 = $(addprefix $(DIROBJ), $(FILE_SHA256:.c=.o))

OBJBASE64 = $(addprefix $(DIROBJ), $(FILE_BASE64:.c=.o))

all : $(NAME)

$(NAME): $(DIROBJ) $(OBJ) $(OBJMD5) $(OBJSHA256) $(OBJBASE64)
	make -C $(LIBDIR)
	gcc $(FLAGS) $(OBJ) $(OBJMD5) $(OBJSHA256) $(OBJBASE64) $(FLIB) -o $(NAME)

$(DIROBJ)%.o : $(DIRC)%.c
	gcc $(FLAGS) -c $< -o $@

$(DIROBJ)%.o : $(DIRMD5)%.c
	gcc $(FLAGS) -c $< -o $@

$(DIROBJ)%.o : $(DIRSHA256)%.c
	gcc $(FLAGS) -c $< -o $@

$(DIROBJ)%.o : $(DIRBASE64)%.c
	gcc $(FLAGS) -c $< -o $@

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

