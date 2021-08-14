#include "../include/des.h"

/*
** Самай простой режим шифрования.
** Данный режим не обладает приптоскойстью.
*/
void	mode_simple(t_des *des)
{
	size_t		i;
	uint64_t	block64;

	i = 0;
	while (i < des->size_message)
	{
		block64 = string_to_uinit64(des->message + i) ^ des->key;
		write_uint64_to_output_message(des, block64, i);
		i += 8;
	}
}

/*
** Режим счетчика.
** В режиме шифрования и расшифрования вектор инициализации только ШИФРУЕТСЯ
** Поэтому можно заранее сделать расчет всех ключей.
*/
void	mode_des_crt(t_des *des)
{
	size_t		i;
	uint64_t	block64;

	i = 0;
	if (des->flags[des_e] == 0)
		des->flags[des_d] = 0;
	while (i < des->size_message)
	{
		block64 = function_des(des, des->init_vector + i);
		block64 = block64 ^ string_to_uinit64(des->message + i);
		write_uint64_to_output_message(des, block64, i);
		i += 8;
	}
	if (des->flags[des_e] == 0)
		des->flags[des_d] = 1;
}

/*
** Режим обратной связи по выходу.
** В режиме шифрования и расшифрования вектор инициализации только ШИФРУЕТСЯ
** Поэтому можно заранее сделать расчет всех ключей.
*/
void	mode_des_cfb(t_des *des)
{
	size_t		i;
	uint64_t	block64;

	i = 0;
	while (i < des->size_message)
	{
		if (des->flags[des_e] == 0)
			des->flags[des_d] = 0;
		block64 = function_des(des, des->init_vector);
		if (des->flags[des_e] == 0)
			des->flags[des_d] = 1;
		if (des->flags[des_e])
		{
			block64 = block64 ^ string_to_uinit64(des->message + i);
			des->init_vector = block64;
		}
		else
		{
			des->init_vector = string_to_uinit64(des->message + i);
			block64 = block64 ^ des->init_vector;
		}
		write_uint64_to_output_message(des, block64, i);
		i += 8;
	}
}

/*
** Режим обратной связи по выходу.
** В режиме шифрования и расшифрования вектор инициализации только ШИФРУЕТСЯ
** Поэтому можно заранее сделать расчет всех ключей.
*/
void	mode_des_ofb(t_des *des)
{
	size_t		i;
	uint64_t	block64;
	uint64_t	plaintext;

	i = 0;
	if (des->flags[des_e] == 0)
		des->flags[des_d] = 0;
	while (i < des->size_message)
	{
		block64 = function_des(des, des->init_vector);
		des->init_vector = block64;
		plaintext = string_to_uinit64(des->message + i);
		block64 = block64 ^ plaintext;
		write_uint64_to_output_message(des, block64, i);
		i += 8;
	}
	if (des->flags[des_e] == 0)
		des->flags[des_d] = 1;
}

/*
** Режим распространяющегося сцепления блока шифка.
** Предыдущий блок открытого текста и предыдущий блок шифротекста
** подвергаются операции XOR с текущим блоком открытого текста перед
** шифрование или после него.
*/
void	mode_des_pcbc(t_des *des)
{
	size_t		i;
	uint64_t	block64;
	uint64_t	plaintext64;

	i = 0;
	while (i < des->size_message)
	{
		block64 = string_to_uinit64(des->message + i);
		plaintext64 = block64;
		if (des->flags[des_e])
			block64 = block64 ^ des->init_vector;
		block64 = function_des(des, block64);
		if (des->flags[des_e])
			des->init_vector = block64 ^ plaintext64;
		if (des->flags[des_d])
			block64 = block64 ^ des->init_vector;
		write_uint64_to_output_message(des, block64, i);
		if (des->flags[des_d])
			des->init_vector = block64 ^ plaintext64;
		i += 8;
	}
}
