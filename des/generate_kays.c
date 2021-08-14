#include "../include/des.h"

/*
** Генерация ключей (16 ключей по 48 бит)
*/
void	generate_kays(t_des *des)
{
	int	i;

	i = -1;
	working_pc1(des);
	des->c0 = (des->key56 >> 28) & 0xFFFFFFF;
	des->d0 = des->key56 & 0xFFFFFFF;
	while (++i < 16)
	{
		shift_to_left_28(&des->c0, &des->d0, i);
		working_pc2(des, i);
	}
}

/*
** Выравнивание сообщения до длины кратной 8 октетам.
*/
void	resize_message(t_des *des)
{
	uint8_t	*temp;

	if (des->size_message % 8 != 0)
	{
		temp = (uint8_t *)ft_strnew(des->size_message
				+ (8 - (des->size_message % 8)));
		ft_memcpy(temp, des->message, des->size_message);
		des->size_message += (8 - (des->size_message % 8));
		free(des->message);
		des->message = temp;
	}
	des->output_message = (uint8_t *)ft_strnew(des->size_message + 2);
}

/*
** Получение сообщения для шифрования из файла.
*/
void	working_file_des(t_des *des)
{
	int		fd;

	fd = open(des->input_file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error open file: ", 2);
		sys_err(des->input_file);
	}
	else
	{
		des->message = (uint8_t *)get_data_fd(fd, &des->size_message);
		close(fd);
	}
}

/*
** Получение сообщения для шифрования из входного потока.
*/
void	working_input_des(t_des *des)
{
	des->message = (uint8_t *)get_data_fd(0, &des->size_message);
}

/*
** Получение сообщения для шифрования из файла или из входного потока.
*/
void	get_message(t_des *des)
{
	if (des->flags[des_i])
		working_file_des(des);
	else
		working_input_des(des);
}
