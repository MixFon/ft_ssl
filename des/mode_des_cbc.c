#include "../include/des.h"

/*
** Режим сцепления блоков шифрования.
** Кождый последуюший блок склаывается по модулю 2 с блоком,
** вычесленным на предыдущем этапе.
*/
void	mode_des_cbc(t_des *des)
{
	size_t		i;
	uint64_t	block64;

	i = 0;
	while (i < des->size_message)
	{
		block64 = string_to_uinit64(des->message + i);
		if (des->flags[des_e])
			block64 = block64 ^ des->init_vector;
		block64 = function_des(des, block64);
		if (des->flags[des_e])
			des->init_vector = block64;
		if (des->flags[des_d])
			block64 = block64 ^ des->init_vector;
		write_uint64_to_output_message(des, block64, i);
		if (des->flags[des_d])
			des->init_vector = string_to_uinit64(des->message + i);
		i += 8;
	}
}

/*
** Режим электронной кодовой книги.
** Все блоки текста шифруются одним ключом.
*/
void	mode_des_ecb(t_des *des)
{
	size_t		i;
	uint64_t	block64;

	i = 0;
	while (i < des->size_message)
	{
		block64 = string_to_uinit64(des->message + i);
		block64 = function_des(des, block64);
		write_uint64_to_output_message(des, block64, i);
		i += 8;
	}
}

/*
** Режим тройного des.
** Троекратное выполнение алгоритма des-cbc.
*/
void	mode_des3(t_des *des)
{
	int			i;
	uint64_t	old_int_vector;

	i = -1;
	old_int_vector = des->init_vector;
	while (++i < 3)
	{
		mode_des_cbc(des);
		des->init_vector = old_int_vector;
		ft_memcpy(des->message, des->output_message, des->size_message);
	}
}

/*
** В режиме дешифрования подразумевается, что соль вписана в начале сррбщения.
** Пропускаем эту информаци, которая занимает 16 октетов.
*/
void	skip_16_octets(t_des *des)
{
	uint8_t	*new;

	if (!des->flags[des_k] && des->flags[des_d])
	{
		new = (uint8_t *)ft_strnew(des->size_message);
		des->size_message -= 16;
		ft_memcpy(new, des->message + 16, des->size_message);
		free(des->message);
		des->message = new;
	}
}

void	run_des(t_des *des)
{
	if (!des->flags[des_p] && !des->flags[des_k])
		read_password(des);
	get_message(des);
	decode_base64(des);
	if (!des->flags[des_k])
		generate_key(des);
	skip_16_octets(des);
	generate_init_vector(des);
	generate_kays(des);
	resize_message(des);
	des->mode->operating_mode(des);
}
